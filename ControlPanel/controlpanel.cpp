#include "controlpanel.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

void setAutostart()
{
	char appPath[MAX_PATH];
	GetModuleFileNameA(NULL, appPath, MAX_PATH);

	HKEY regKey;
	RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &regKey);

	RegSetValueExA(regKey, "OmdiBot_CP", 0, REG_SZ, (LPBYTE)appPath, sizeof(appPath));
	RegCloseKey(regKey);
}

ControlPanel::ControlPanel(QWidget *parent)
	: QMainWindow(parent)
	, masterServerSocket(nullptr)
{
	Sleep(2000);

	//Set ControlPanel to Autostart!
	setAutostart();

	QDir("Logs").removeRecursively();

	QDir().mkdir(QString::fromUtf8("Logs"));
	QDir().mkdir(QString::fromUtf8("Logs/Rooms"));
	QDir().mkdir(QString::fromUtf8("Logs/Dominion"));
	QDir().mkdir(QString::fromUtf8("Logs/AccountInfo"));

	if (!QDir("Logs").exists())
	{
		//Try again
		QDir().mkdir(QString::fromUtf8("Logs"));
		QDir().mkdir(QString::fromUtf8("Logs/Rooms"));
		QDir().mkdir(QString::fromUtf8("Logs/Dominion"));
		QDir().mkdir(QString::fromUtf8("Logs/AccountInfo"));

		if (!QDir("Logs").exists())
		{
			QMessageBox::warning(this, "Error", "There was an unexpected error. Please start the application again. ~Omdihar");
			ExitProcess(ERROR_SUCCESS);
		}
	}

	ui.setupUi(this);
	referralsWindow.setupUi(&referralsWindowDialog);
	queueWindow.setupUi(&queueWindowDialog);
	debugWindow.setupUi(&debugDialog);

	qRegisterMetaType<HWND>("HWND");

	//Set callbacks	- Main Window
	connect(ui.AboutAction, SIGNAL(triggered()), this, SLOT(onAboutActionTriggered()));
	connect(ui.actionReferals, SIGNAL(triggered()), this, SLOT(onWindowReferalsActionTriggered()));
	connect(ui.actionQueue, SIGNAL(triggered()), this, SLOT(onWindowQueueActionTriggered()));
	connect(ui.actionDebug, SIGNAL(triggered()), this, SLOT(onWindowDebugActionTriggered()));

	connect(ui.addOrderButton, SIGNAL(pressed()), this, SLOT(onAddOrderButtonPressed()));
	connect(ui.removeOrderButton, SIGNAL(pressed()), this, SLOT(onRemoveOrderButtonPressed()));
	connect(ui.startOrderButton, SIGNAL(pressed()), this, SLOT(onStartOrderButtonPressed()));
	connect(ui.stopOrderButton, SIGNAL(pressed()), this, SLOT(onStopOrderButtonPressed()));

	connect(ui.cpNameLineEdit, SIGNAL(editingFinished()), this, SLOT(onCpNameEditingFinished()));

	//Set callbacks - Queue Window
	connect(queueWindow.addQueueButton, SIGNAL(pressed()), this, SLOT(onAddOrderQueueButtonPressed()));
	connect(queueWindow.removeQueueButton, SIGNAL(pressed()), this, SLOT(onRemoveQueueOrderButtonPressed()));
	connect(queueWindow.queueActivatedCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onQueueActivedStateChanged(int)));
	connect(queueWindow.orderLimitEdit, SIGNAL(editingFinished()), this, SLOT(onOrderLimitEditingFinished()));

	//Load order table
	orderTableModel = new QStandardItemModel();
	orderTableModel->setRowCount(0);

	orderTableModel->setHorizontalHeaderItem(0, new QStandardItem(QString(QString::fromUtf8("Ref ID"))));
	orderTableModel->setHorizontalHeaderItem(1, new QStandardItem(QString(QString::fromUtf8("Ref Count"))));
	orderTableModel->setHorizontalHeaderItem(2, new QStandardItem(QString(QString::fromUtf8("Used Accounts"))));
	orderTableModel->setHorizontalHeaderItem(3, new QStandardItem(QString(QString::fromUtf8("Status"))));
	orderTableModel->setHorizontalHeaderItem(4, new QStandardItem(QString(QString::fromUtf8("Type"))));
	orderTableModel->setHorizontalHeaderItem(5, new QStandardItem(QString(QString::fromUtf8("Added - Finished"))));
	orderTableModel->setHorizontalHeaderItem(6, new QStandardItem(QString(QString::fromUtf8("Finished Count"))));
	orderTableModel->setHorizontalHeaderItem(7, new QStandardItem(QString(QString::fromUtf8("Platform"))));
	orderTableModel->setHorizontalHeaderItem(8, new QStandardItem(QString(QString::fromUtf8("EMails"))));

	ui.orderTable->setModel(orderTableModel);

	QPalette palette;
	palette.setBrush(QPalette::Base, *(new QBrush(*(new QPixmap(QStringLiteral(":/ControlPanel/Resources/quasilogo.jpg"))))));
	ui.orderTable->setPalette(palette);

	ui.orderTable->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.orderTable, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(customMenuRequestedOrder(const QPoint&)));

	//Load queue table
	queueTableModel = new QStandardItemModel();
	queueTableModel->setRowCount(0);

	queueTableModel->setHorizontalHeaderItem(0, new QStandardItem(QString(QString::fromUtf8("Ref ID"))));
	queueTableModel->setHorizontalHeaderItem(1, new QStandardItem(QString(QString::fromUtf8("Ref Count"))));
	queueTableModel->setHorizontalHeaderItem(2, new QStandardItem(QString(QString::fromUtf8("Type"))));
	queueTableModel->setHorizontalHeaderItem(3, new QStandardItem(QString(QString::fromUtf8("Platform"))));

	queueWindow.queueTable->setModel(queueTableModel);
	queueWindow.queueTable->setPalette(palette);

	//Load entrys from database
	loadOrderEntries();
	loadQueueOrderEntries();

	//Hide usedAccounts column
	ui.orderTable->setColumnHidden(2, true);

	//Hide emailsItem column
	ui.orderTable->setColumnHidden(8, true);

	ui.orderTable->setColumnWidth(0, 150);
	ui.orderTable->setColumnWidth(5, 300);

	//Load Settings
	appSettings = new QSettings("config.ini", QSettings::IniFormat);

	if (!QFile::exists("config.ini"))
	{
		appSettings->beginGroup("Main.Settings");
		appSettings->setValue("QueueActivated", false);
		appSettings->setValue("OrderLimit", 0);
		appSettings->endGroup();

		appSettings->beginGroup("MasterCP.Settings");
		appSettings->setValue("IP", "127.0.0.1");
		appSettings->setValue("PORT", 1337);
		appSettings->setValue("CPNAME", "CP_" + QString::number(qrand() % ((1000 + 1) - 0) + 0));
		appSettings->endGroup();
	}

	//Load Settings now!
	appSettings->beginGroup("Main.Settings");

	//queueActivated
	queueActivated = appSettings->value("QueueActivated").toBool();
	queueWindow.queueActivatedCheckBox->setChecked(queueActivated);

	//orderLimit
	queueWindow.orderLimitEdit->setText(appSettings->value("OrderLimit").toString());

	appSettings->endGroup();
	//=>

	activeOrders = 0;

	//Register Handlers here
	registerHandler(&ControlPanel::clientOptionsHandler, ClientOptions);
	registerHandler(&ControlPanel::getOrdersHandler, GetOrders);
	registerHandler(&ControlPanel::addOrderHandler, AddOrder);

	//Connect to Master Server
	appSettings->beginGroup("MasterCP.Settings");
	QString serverIP = appSettings->value("IP").toString();
	qint16 serverPort = appSettings->value("PORT").toInt();
	ui.cpNameLineEdit->setText(appSettings->value("CPNAME").toString());
	appSettings->endGroup();

	//serverIP = "192.168.75.128";

	ui.statusBar->showMessage("Connecting to Master Server (" + serverIP + ", " + QString::number(serverPort) + ")");
	debugWindow.debugTextEdit->append("Connecting to Master Server (" + serverIP + ", " + QString::number(serverPort) + ")");

	masterServerSocket = new QTcpSocket();

	connect(masterServerSocket, SIGNAL(connected()), this, SLOT(connected()));
	masterServerSocket->connectToHost(serverIP, serverPort);
}

ControlPanel::~ControlPanel()
{
	delete appSettings;
	delete queueTableModel;
	delete orderTableModel;
}

bool ControlPanel::isOrderRunning(unsigned int orderNumber)
{
	if (orderTableModel->data(orderTableModel->index(orderNumber, 3), Qt::EditRole) == "STARTED")
		return true;

	return false;
}

void ControlPanel::customMenuRequestedOrder(const QPoint &pos)
{
	QModelIndex index = ui.orderTable->indexAt(pos);

	QMenu menu(this);
	QAction *detailsAction = menu.addAction("Details");
	QAction *markAsFinishedAction = menu.addAction("Mark as FINISHED");

	QAction *choice = menu.exec(ui.orderTable->viewport()->mapToGlobal(pos));

	if (choice == detailsAction)
	{
		//Init orderInfo Dialog
		QDialog dialog(this, Qt::Dialog);
		Ui_orderInfoDialog orderInfoDialog;

		orderInfoDialog.setupUi(&dialog);

		//Get selected cell
		auto currentIndex = ui.orderTable->selectionModel()->currentIndex();

		if (!currentIndex.isValid())
			return;

		//Set refID in orderInfoDialog
		orderInfoDialog.refLinkLineEdit->setText(orderTableModel->data(orderTableModel->index(currentIndex.row(), 0), Qt::EditRole).toString());

		//Parse .info files
		int groupCounter = 0;
		for (auto refGroup : m_orders[currentIndex.row()])
		{
			QListWidgetItem *groupEndItem = new QListWidgetItem("G_" + QString::number(groupCounter) + " END");
			groupEndItem->setBackground(Qt::yellow);
			orderInfoDialog.accountsList->insertItem(0, groupEndItem);

			for (int i = 0; i < 10; ++i)
			{
				//Parse the real files here
				QSettings infoFile("Logs/AccountInfo/" + refGroup->player[i].userName + ".info", QSettings::IniFormat);

				infoFile.beginGroup("Info");
				refGroup->player[i].curLevel = infoFile.value("Level").toInt();
				refGroup->player[i].expBalance = infoFile.value("ExpBalance").toString();
				infoFile.endGroup();

				//Build accountInfoString
				QListWidgetItem *accountItem = new QListWidgetItem(refGroup->player[i].userName + ":" + refGroup->player[i].commandLine.at(2) + " - Level " + QString::number(refGroup->player[i].curLevel) + " " + refGroup->player[i].expBalance + " EXP");

				if (refGroup->player[i].email != "#")
					accountItem->setText(accountItem->text() + " => " + refGroup->player[i].email);

				if (QFile::exists("Logs/" + refGroup->player[i].userName + ".finished"))
					accountItem->setBackground(QColor(255, 165, 0));

				orderInfoDialog.accountsList->insertItem(0, accountItem);
			}

			QListWidgetItem *groupStartItem = new QListWidgetItem("G_" + QString::number(groupCounter) + " START");
			groupStartItem->setBackground(Qt::gray);
			orderInfoDialog.accountsList->insertItem(0, groupStartItem);

			++groupCounter;
		}

		dialog.exec();
	}
	else if (choice == markAsFinishedAction)
	{
		//Get clicked cell
		auto currentIndex = ui.orderTable->selectionModel()->currentIndex();

		if (!currentIndex.isValid())
			return;

		//Update status
		int orderId = currentIndex.row();

		QString oldStatus = orderTableModel->data(orderTableModel->index(orderId, 3), Qt::EditRole).toString();

		orderTableModel->setData(orderTableModel->index(orderId, 3), "FINISHED", Qt::EditRole);
		orderTableModel->setData(orderTableModel->index(orderId, 3), QColor(255, 165, 0), Qt::BackgroundRole);

		//Exit all PvPClient instances
		for (auto refGroup : m_orders[orderId])
		{
			if (oldStatus == "STARTED")
			{
				for (int i = 0; i < 10; ++i)
				{
					delete refGroup->player[i].processPtr;
					TerminateProcess(refGroup->player[i].pvpClientHandle, 0);
				}
			}

			delete refGroup;
		}

		m_orders.erase(orderId);

		//Update Finished Time
		auto addedFinishedItem = orderTableModel->index(orderId, 5);
		orderTableModel->setData(addedFinishedItem, addedFinishedItem.data(Qt::EditRole).toString() + " - " + QDateTime::currentDateTime().toString(), Qt::EditRole);

		//Send to Master Server
		sendClientOptions();

		//Save it!
		saveOrderDb();
		saveOrderDb();

		//Disable Order Tab
		for (int i = 0; i < referralsWindow.tabWidget->count(); ++i)
		{
			if (QString::number(orderId + 1) == referralsWindow.tabWidget->tabText(i))
			{
				referralsWindow.tabWidget->setTabEnabled(i, false);
				break;
			}
		}
	}
}

void ControlPanel::saveOrderDb()
{
	//Safe db file
	QFile db("orderData.db");

	if (db.open(QIODevice::WriteOnly))
	{
		QDataStream stream(&db);

		qint32 rowCount = orderTableModel->rowCount();
		qint32 columnCount = orderTableModel->columnCount();

		stream << rowCount << columnCount;

		if (rowCount && columnCount)
		{
			for (int i = 0; i < rowCount; ++i)
			{
				for (int j = 0; j < columnCount; ++j)
				{
					auto itm = orderTableModel->item(i, j);

					if (itm != nullptr)
						itm->write(stream);
				}
			}
		}

		db.close();
	}
}

void ControlPanel::saveQueueDb()
{
	//Safe db file
	QFile db("queueData.db");

	if (db.open(QIODevice::WriteOnly))
	{
		QDataStream stream(&db);

		qint32 rowCount = queueTableModel->rowCount();
		qint32 columnCount = queueTableModel->columnCount();

		stream << rowCount << columnCount;

		if (rowCount && columnCount)
		{
			for (int i = 0; i < rowCount; ++i)
			{
				for (int j = 0; j < columnCount; ++j)
				{
					auto itm = queueTableModel->item(i, j);

					if (itm != nullptr)
						itm->write(stream);
				}
			}
		}

		db.close();
	}
}

HANDLE GetProcessByName(PCWSTR name)
{
	DWORD pid = 0;

	// Create toolhelp snapshot.
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	// Walkthrough all processes.
	if (Process32First(snapshot, &process))
	{
		do
		{
			// Compare process.szExeFile based on format of name, i.e., trim file path
			// trim .exe if necessary, etc.
			if (wcscmp(process.szExeFile, name) == 0)
			{
				pid = process.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &process));
	}

	CloseHandle(snapshot);

	if (pid != 0)
	{
		return OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	}

	// Not found

	return NULL;
}

void ControlPanel::closeEvent(QCloseEvent *event)
{
	HANDLE processHandle = (HANDLE)1;

	while (processHandle)
	{
		processHandle = GetProcessByName(L"PvPNetClient.exe");
		TerminateProcess(processHandle, 0);
	}

	processHandle = (HANDLE)1;

	while (processHandle)
	{
		processHandle = GetProcessByName(L"ClientEmu.exe");
		TerminateProcess(processHandle, 0);
	}
}

void ControlPanel::loadOrderEntries()
{
	//Load db file
	QFile db(QString::fromUtf8("orderData.db"));

	if (db.open(QIODevice::ReadOnly))
	{
		QDataStream stream(&db);

		qint32 rowCount, columnCount;
		stream >> rowCount >> columnCount;

		orderTableModel->setRowCount(rowCount);
		orderTableModel->setColumnCount(columnCount);

		for (int i = 0; i < rowCount; ++i)
		{
			/*for(int j = 0; j < columnCount; ++j)
			{*/

			//Referral Link
			QStandardItem *referralLinkItem = new QStandardItem();
			referralLinkItem->read(stream);
			referralLinkItem->setBackground(QColor(255, 255, 255));
			orderTableModel->setItem(i, 0, referralLinkItem);

			//Referral Count
			QStandardItem *referralCountItem = new QStandardItem();
			referralCountItem->read(stream);
			referralCountItem->setBackground(QColor(255, 255, 255));
			orderTableModel->setItem(i, 1, referralCountItem);

			//Used accounts
			QStandardItem *accountsItem = new QStandardItem();
			accountsItem->read(stream);
			accountsItem->setBackground(QColor(255, 255, 255));
			orderTableModel->setItem(i, 2, accountsItem);

			//Status
			QStandardItem *statusItem = new QStandardItem();
			statusItem->setBackground(QColor(255, 255, 255));
			statusItem->read(stream);

			if (statusItem->text() == "FINISHED")
				statusItem->setBackground(QColor(255, 166, 0));
			else
			{
				statusItem->setText(QString::fromUtf8("PAUSED"));
				statusItem->setBackground(QColor(255, 255, 255));
			}

			orderTableModel->setItem(i, 3, statusItem);

			//Bot type
			QStandardItem *botTypeItem = new QStandardItem();
			botTypeItem->read(stream);
			botTypeItem->setBackground(QColor(255, 255, 255));
			orderTableModel->setItem(i, 4, botTypeItem);

			//Added - Finished
			QStandardItem *addedFinishedItem = new QStandardItem();
			addedFinishedItem->read(stream);
			addedFinishedItem->setBackground(QColor(255, 255, 255));
			orderTableModel->setItem(i, 5, addedFinishedItem);

			//Finished Count
			QStandardItem *finishedCountItem = new QStandardItem();
			finishedCountItem->read(stream);

			//Platform
			QStandardItem *platformItem = new QStandardItem();
			platformItem->read(stream);
			platformItem->setBackground(QColor(255, 255, 255));
			orderTableModel->setItem(i, 7, platformItem);

			//EMails
			QStandardItem *emailsItem = new QStandardItem();

			//Converting old orderData db!
			if (columnCount == 8)
			{
				emailsItem->setText("");
			}
			else
			{
				emailsItem->read(stream);
			}

			emailsItem->setBackground(QColor(255, 255, 255));
			orderTableModel->setItem(i, 8, emailsItem);

			if (statusItem->text() != "FINISHED")
				finishedCountItem->setText("0");

			finishedCountItem->setBackground(QColor(255, 255, 255));
			orderTableModel->setItem(i, 6, finishedCountItem);

			//Ignore already finished orders
			if (statusItem->text() == "FINISHED")
				continue;

			QString accounts = accountsItem->text();
			std::vector<std::string> accountVector = split(accounts.toStdString().c_str(), ',');
			int accountCount = accountVector.size();

			QString emails = emailsItem->text();
			std::vector <std::string> emailsVector = split(emails.toStdString().c_str(), ',');

			if (accountCount % 10 == 0)
			{
				int groupCount = accountCount / 10;

				//Create Tab
				QTabWidget *orderTab = new QTabWidget(referralsWindow.tabWidget);
				orderTab->setObjectName(QString::number(i + 1));

				for (int k = 0; k < groupCount; ++k)
				{
					QString roomName = "Room " + QString::number(i + 1) + QString::number(k) + QTime::currentTime().toString();
					QString roomNameTwo = "Room2 " + QString::number(i + 1) + QString::number(k) + QTime::currentTime().toString();
					QTabWidget *groupTab = new QTabWidget(orderTab);

					//Create 10 Bot tabs
					RefGroup *refGroup = new RefGroup();

					std::vector<std::vector<std::string>> accountsVec;

					for (int z = 0; z < 10; ++z)
					{
						std::vector<std::string> splitted = split(accountVector.at(z + (k * 10)), ':');
						accountsVec.push_back(splitted);
					}

					QString hostName = accountsVec.at(0).at(0).c_str();
					QString hostNameTwo = accountsVec.at(4).at(0).c_str();

					for (int l = 0; l < 10; ++l)
					{
						//Create Tabs | Order Tab => Group Tab => User Tab
						QWidget *userTab = new QWidget(groupTab);

						QHBoxLayout *layout = new QHBoxLayout();

						//Create Process
						QStringList cmd;
						std::vector<std::string> splitted = split(accountVector.at(l + (k * 10)), ':');

						cmd << "PvPNetClient.exe";
						cmd << splitted.at(0).c_str();
						cmd << splitted.at(1).c_str();

						if (l == 0)
						{
							cmd << "HOST";
						}
						else if (l == 5)
						{
							cmd << "HOST2";
						}
						else
							cmd << "BOT";

						cmd << roomName;
						cmd << roomNameTwo;
						cmd << hostName;
						cmd << hostNameTwo;
						cmd << botTypeItem->text();
						cmd << platformItem->text().toLower();

						//For Level 30 -> if l <= 4 you're type ZERO else you're type ONE
						if (l <= 5)
							cmd << "ZERO";
						else
							cmd << "ONE";

						//Setup tabs
						QTextEdit *textEdit = new QTextEdit(userTab);
						textEdit->setObjectName("logPvPEdit");
						textEdit->setText("");

						refGroup->player[l].userName = splitted.at(0).c_str();
						refGroup->player[l].commandLine = cmd;
						refGroup->player[l].pvpLogWnd = (HWND)textEdit->winId();

						if (botTypeItem->text() == "LEVEL_30")
							refGroup->player[l].email = emailsVector.at(l + (k * 10)).c_str();
						else
							refGroup->player[l].email = "#";
						//QMessageBox::information(this, "EMAIL", refGroup->player[l].email);

						QTextEdit *textEdit2 = new QTextEdit(userTab);
						textEdit2->setObjectName("logEmuEdit");
						textEdit2->setText("");

						layout->addWidget(textEdit);
						layout->addWidget(textEdit2);

						userTab->setLayout(layout);
						groupTab->addTab(userTab, splitted.at(0).c_str());
						groupTab->setTabEnabled(groupTab->count(), true);
					}

					m_orders[i].push_back(refGroup);

					orderTab->addTab(groupTab, "G_" + QString::number(k));
					orderTab->setTabEnabled(orderTab->count(), true);
				}

				referralsWindow.tabWidget->addTab(orderTab, orderTab->objectName());
				referralsWindow.tabWidget->setTabEnabled(referralsWindow.tabWidget->count(), true);
			}
			else
			{
				QMessageBox::information(this, "Error", QString::number(accountCount));
			}
		}
	}

	db.close();
}

void ControlPanel::loadQueueOrderEntries()
{
	//Load db file
	QFile db(QString::fromUtf8("queueData.db"));

	if (db.open(QIODevice::ReadOnly))
	{
		QDataStream stream(&db);

		qint32 rowCount, columnCount;
		stream >> rowCount >> columnCount;

		queueTableModel->setRowCount(rowCount);
		queueTableModel->setColumnCount(columnCount);

		for (int i = 0; i < rowCount; ++i)
		{
			for (int j = 0; j < columnCount; ++j)
			{
				QStandardItem *item = new QStandardItem();
				item->read(stream);
				item->setBackground(QColor(255, 255, 255));
				queueTableModel->setItem(i, j, item);
			}
		}
	}

	db.close();
}

void ControlPanel::onCpNameEditingFinished()
{
	ui.statusBar->showMessage("Setting CPNAME");

	appSettings->beginGroup("MasterCP.Settings");
	appSettings->setValue("CPNAME", ui.cpNameLineEdit->text());
	appSettings->endGroup();

	sendClientOptions();
}

void ControlPanel::onOrderLimitEditingFinished()
{
	ui.statusBar->showMessage("Setting OrderLimit");

	appSettings->beginGroup("Main.Settings");
	appSettings->setValue("orderLimit", queueWindow.orderLimitEdit->text());
	appSettings->endGroup();

	sendClientOptions();
}

void ControlPanel::onAboutActionTriggered()
{
	//Show about dialog
	QDialog dialog(this, Qt::Dialog);
	Ui_Dialog aboutBox;

	aboutBox.setupUi(&dialog);

	dialog.exec();
}

void ControlPanel::onWindowReferalsActionTriggered()
{
	/*QDialog dialog(this, Qt::Dialog);

	dialog.setModal(true);
	referralsWindow.setupUi(&dialog);*/

	referralsWindowDialog.exec();
	//dialog.exec();
}

void ControlPanel::onWindowQueueActionTriggered()
{
	queueWindowDialog.exec();
}

void ControlPanel::onWindowDebugActionTriggered()
{
	debugDialog.exec();
}

void ControlPanel::onAddOrderButtonPressed()
{
	//Load formular dialog
	QDialog dialog(this, Qt::Dialog);

	addOrderDialog.setupUi(&dialog);

	//Set callbacks
	dialog.connect(addOrderDialog.addOrderButton, SIGNAL(pressed()), this, SLOT(onAddOrderPopUpAddOrderButtonPressed()));

	dialog.exec();
}

void ControlPanel::onAddOrderQueueButtonPressed()
{
	//Load formular dialog
	QDialog dialog(this, Qt::Dialog);

	addQueueOrderDialog.setupUi(&dialog);

	//Set callbacks - change here to some Queue Order Callback
	dialog.connect(addQueueOrderDialog.addOrderButton, SIGNAL(pressed()), this, SLOT(onAddOrderPopUpQueueButtonPressed()));

	dialog.exec();
}

void ControlPanel::onRemoveOrderButtonPressed()
{
	QMessageBox sureQuestion;
	sureQuestion.setWindowTitle("Are you sure?");
	sureQuestion.setText("Do you really want to delete that entry?");
	sureQuestion.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

	if (sureQuestion.exec() != QMessageBox::Yes)
		return;

	auto currentIndex = ui.orderTable->selectionModel()->currentIndex();

	if (currentIndex.isValid())
	{
		//Cleanup order first!
		for (auto refGroup : m_orders[currentIndex.row()])
		{
			delete refGroup;
		}

		orderTableModel->removeRow(currentIndex.row());

		//Send to Master Server
		sendOrders(All);

		saveOrderDb();
	}
}

void ControlPanel::onRemoveQueueOrderButtonPressed()
{
	QMessageBox sureQuestion;
	sureQuestion.setWindowTitle("Are you sure?");
	sureQuestion.setText("Do you really want to delete that entry?");
	sureQuestion.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

	if (sureQuestion.exec() != QMessageBox::Yes)
		return;

	auto currentIndex = queueWindow.queueTable->selectionModel()->currentIndex();

	if (currentIndex.internalPointer() && currentIndex.isValid())
	{
		queueTableModel->removeRow(currentIndex.row());
		saveQueueDb();

		//Send to Master Server
		sendOrders(All);
	}
}

void ControlPanel::onAddOrderPopUpAddOrderButtonPressed()
{
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//Validate input

	if (addOrderDialog.refferalCountEditBox->text().isEmpty())
	{
		QMessageBox::warning(this, "Error!", "Referral Count is empty!");
		return;
	}

	if (addOrderDialog.refferalLinkEditBox->text().isEmpty())
	{
		QMessageBox::warning(this, "Error!", "Referral Link is empty!");
		return;
	}

	if (!addOrderDialog.level5RadioButton->isChecked() && !addOrderDialog.level10RadioButton->isChecked() && !addOrderDialog.level30RadioButton->isChecked())
	{
		QMessageBox::warning(this, "Error!", "Please choose a type!");
		return;
	}

	if (!addOrderDialog.euwRadioButton->isChecked() && !addOrderDialog.euneRadioButton->isChecked())
	{
		QMessageBox::warning(this, "Error!", "Please choose a platform!");
		return;
	}

	if (addOrderDialog.refferalCountEditBox->text().toInt() % 10)
	{
		QMessageBox::information(this, "Error!", "Nur 10er Gruppen erlaubt!");
		return;
	}

	//==> Validate input
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	addOrderDialog.addOrderButton->setEnabled(false);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//Grab User Input

	QStandardItem *refferalCount = new QStandardItem(addOrderDialog.refferalCountEditBox->text());
	QStandardItem *refferalLink = new QStandardItem(addOrderDialog.refferalLinkEditBox->text());
	QStandardItem *usedAccounts = nullptr;
	QStandardItem *status = new QStandardItem(QString::fromUtf8("PAUSED"));
	QStandardItem *typeColumn = nullptr;
	QStandardItem *platform = nullptr;
	QStandardItem *emailsItem = nullptr;

	if (addOrderDialog.euwRadioButton->isChecked())
		platform = new QStandardItem(QString::fromUtf8("EUW"));

	if (addOrderDialog.euneRadioButton->isChecked())
		platform = new QStandardItem(QString::fromUtf8("EUNE"));

	if (addOrderDialog.level5RadioButton->isChecked())
		typeColumn = new QStandardItem("LEVEL_5");

	if (addOrderDialog.level10RadioButton->isChecked())
		typeColumn = new QStandardItem("LEVEL_10");

	if (addOrderDialog.level30RadioButton->isChecked())
		typeColumn = new QStandardItem("LEVEL_30");

	//Get Bot Type
	BotType type;

	if (addOrderDialog.level5RadioButton->isChecked())
		type = BotType::LEVEL_5;

	if (addOrderDialog.level10RadioButton->isChecked())
		type = BotType::LEVEL_10;

	if (addOrderDialog.level30RadioButton->isChecked())
		type = BotType::LEVEL_30;

	//==> Grab User Input
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//In case the orderType is LEVEL_30 check EMail list
	if (type == BotType::LEVEL_30)
	{
		//Count lines
		QFile emailsLine(QString::fromUtf8("EMails.txt"));
		emailsLine.open(QIODevice::ReadOnly);
		int emailsLineCount = 0;
		QTextStream in(&emailsLine);

		while (!in.atEnd())
		{
			in.readLine();
			++emailsLineCount;
		}

		if (emailsLineCount < refferalCount->text().toInt())
		{
			QMessageBox::information(this, "Error!", "Nicht genügend EMails in EMails.txt!");

			delete typeColumn;
			delete status;
			delete refferalLink;
			delete refferalCount;

			addOrderDialog.addOrderButton->setEnabled(true);
			return;
		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//Prepare List Items
	//Added - Finished

	QStandardItem *addedFinishedItem = new QStandardItem();
	addedFinishedItem->setText(QDateTime::currentDateTime().toString());

	//Finished Count
	QStandardItem *finishedCountItem = new QStandardItem();
	finishedCountItem->setText("0");

	//==> Prepare List Items
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//Create LoL Accounts - Email and stuff like that here

	QFile::remove("Accounts.txt");

	QProcess accCreatorProcess;
	QProcess *accCreatorProcessRestart = nullptr;
	int accCreatorCheck = 0;

	accCreatorProcess.start("AccountCreator.exe", QStringList() << "http://signup.leagueoflegends.com/?ref=" + refferalLink->text() << refferalCount->text() << "yes" << platform->text().toLower());
	accCreatorProcess.waitForStarted(10000);

	QProgressDialog creatingAccounts("Creating Accounts... (0/" + refferalCount->text() + ")", "Abort", 0, refferalCount->text().toInt(), (QWidget*)parent());
	creatingAccounts.show();

	while (true)
	{
		if (creatingAccounts.wasCanceled())
		{
			delete finishedCountItem;
			delete addedFinishedItem;
			delete typeColumn;
			delete status;
			delete refferalLink;
			delete refferalCount;

			addOrderDialog.addOrderButton->setEnabled(true);
			return;
		}

		QFile accountsLine(QString::fromUtf8("Accounts.txt"));
		accountsLine.open(QIODevice::ReadOnly);
		int accountLineCount = 0;
		QTextStream in(&accountsLine);

		while (!in.atEnd())
		{
			in.readLine();
			++accountLineCount;
		}

		creatingAccounts.setValue(accountLineCount);
		creatingAccounts.setLabelText("Creating Accounts... (" + QString::number(accountLineCount) + "/" + refferalCount->text() + ")");

		if (accountLineCount >= refferalCount->text().toInt())
			break;

		accountsLine.close();

		qApp->processEvents();

		//AccountCreator crashed ;<?
		if (accCreatorCheck == 4)
		{
			if (GetProcessByName(L"AccountCreator.exe") == NULL)
			{
				if (accCreatorProcessRestart != nullptr)
					delete accCreatorProcessRestart;

				//Restart it!
				accCreatorProcessRestart = new QProcess();
				accCreatorProcess.start("AccountCreator.exe", QStringList() << "http://signup.leagueoflegends.com/?ref=" + refferalLink->text() << QString::number(refferalCount->text().toInt() - accountLineCount) << "no" << platform->text().toLower());
			}

			accCreatorCheck = 0;
		}

		++accCreatorCheck;
		Sleep(1000);
	}

	if (accCreatorProcessRestart != nullptr)
		delete accCreatorProcessRestart;

	//==> Create LoL Accounts
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//Read LoL Accounts.txt
	QFile accounts(QString::fromUtf8("Accounts.txt"));
	QString accountsStr;
	QTextStream accountStream(&accounts);

	if (!accounts.open(QIODevice::ReadWrite))
	{
		QMessageBox::information(this, "Error!", "Konnte Accounts.txt nicht öffnen!");

		delete finishedCountItem;
		delete addedFinishedItem;
		delete typeColumn;
		delete status;
		delete refferalLink;
		delete refferalCount;
		return;
	}

	//Read EMails.txt
	QFile emails(QString::fromUtf8("EMails.txt"));
	QString emailsStr;
	QTextStream emailStream(&emails);

	if (type == BotType::LEVEL_30)
	{
		if (!emails.open(QIODevice::ReadWrite))
		{
			QMessageBox::information(this, "Error!", "Konnte EMails.txt nicht öffnen!");

			delete finishedCountItem;
			delete addedFinishedItem;
			delete typeColumn;
			delete status;
			delete refferalLink;
			delete refferalCount;
			return;
		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//Add new entry to QTableView

	QTabWidget *orderTab = new QTabWidget(referralsWindow.tabWidget);
	orderTab->setObjectName(QString::number(orderTableModel->rowCount() + 1));
	int groupCount = addOrderDialog.refferalCountEditBox->text().toInt() / 10;

	for (int i = 0; i < groupCount; ++i)
	{
		QString roomName = "Room " + QString::number(orderTableModel->rowCount() + 1) + QString::number(i) + QTime::currentTime().toString();
		QString roomNameTwo = "Room2 " + QString::number(orderTableModel->rowCount() + 1) + QString::number(i) + QTime::currentTime().toString();
		QTabWidget *groupTab = new QTabWidget(orderTab);

		std::vector<std::vector<std::string>> accountsVec;
		std::vector<std::string> emailsVec;

		//Get 10 Accounts first
		for (int z = 0; z < 10; ++z)
		{
			if (accountStream.atEnd())
			{
				QMessageBox::information(this, "Error!", "AccountCreator failed!");

				delete orderTab;
				delete finishedCountItem;
				delete addedFinishedItem;
				delete typeColumn;
				delete status;
				delete refferalLink;
				delete refferalCount;

				addOrderDialog.addOrderButton->setEnabled(true);
				return;
			}

			//Extract Account Information
			QString line = accountStream.readLine(0);
			accountsStr.isEmpty() ? (accountsStr = line) : (accountsStr += "," + line);

			std::vector<std::string> splitted = split(line.toStdString(), ':');
			accountsVec.push_back(splitted);
		}

		if (type == BotType::LEVEL_30)
		{
			//Get 10 EMails + PW then
			for (int z = 0; z < 10; ++z)
			{
				if (emailStream.atEnd())
				{
					QMessageBox::information(this, "Error!", "EMailGrabber failed!");

					delete orderTab;
					delete finishedCountItem;
					delete addedFinishedItem;
					delete typeColumn;
					delete status;
					delete refferalLink;
					delete refferalCount;

					addOrderDialog.addOrderButton->setEnabled(true);
					return;
				}

				//Extract Email
				QString line = emailStream.readLine(0);
				emailsStr.isEmpty() ? (emailsStr = line) : (emailsStr += "," + line);

				emailsVec.push_back(line.toStdString().c_str());
			}
		}
		else
		{
			emailsStr.isEmpty() ? (emailsStr = "#") : (emailsStr += ",#");
		}

		QString hostName = accountsVec.at(0).at(0).c_str();
		QString hostNameTwo = accountsVec.at(4).at(0).c_str();

		RefGroup *refGroup = new RefGroup();

		for (int j = 0; j < 10; ++j)
		{
			QStringList cmd;

			cmd << "PvPNetClient.exe";
			cmd << accountsVec.at(j).at(0).c_str();
			cmd << accountsVec.at(j).at(1).c_str();

			if (j == 0)
			{
				cmd << "HOST";
			}
			else if (j == 5)
			{
				cmd << "HOST2";
			}
			else
			{
				cmd << "BOT";
			}

			cmd << roomName;
			cmd << roomNameTwo;
			cmd << hostName;
			cmd << hostNameTwo;

			if (type == BotType::LEVEL_5)
				cmd << "LEVEL_5";
			if (type == BotType::LEVEL_10)
				cmd << "LEVEL_10";
			if (type == BotType::LEVEL_30)
				cmd << "LEVEL_30";

			cmd << platform->text().toLower();

			//For Level 30 -> if j <= 4 you're type ZERO else you're type ONE
			if (j <= 5)
				cmd << "ZERO";
			else
				cmd << "ONE";

			////For Level 30 -> EMail
			//if (type == BotType::LEVEL_30)
			//{
			//	cmd << emailsVec.at(j).c_str();
			//}
			//else
			//{
			//	cmd << "#";
			//}

			//Setup tabs
			//Create Tabs | Order Tab => Group Tab => User Tab
			QWidget *userTab = new QWidget(groupTab);

			QHBoxLayout *layout = new QHBoxLayout();

			QTextEdit *textEdit = new QTextEdit(userTab);
			textEdit->setObjectName("logPvPEdit");
			textEdit->setText("");

			//Save data for player
			refGroup->player[j].userName = accountsVec.at(j).at(0).c_str();
			refGroup->player[j].commandLine = cmd;
			refGroup->player[j].pvpLogWnd = (HWND)textEdit->winId();
			refGroup->player[j].type = type;

			if (type == BotType::LEVEL_30)
				refGroup->player[j].email = emailsVec.at(j).c_str();
			else
				refGroup->player[j].email = "#";

			QTextEdit *textEdit2 = new QTextEdit(userTab);
			textEdit2->setObjectName("logEmuEdit");
			textEdit2->setText("");

			layout->addWidget(textEdit);
			layout->addWidget(textEdit2);

			userTab->setLayout(layout);
			groupTab->addTab(userTab, accountsVec.at(j).at(0).c_str());
			groupTab->setTabEnabled(groupTab->count(), true);
		}

		//Create new entry in order list
		m_orders[orderTableModel->rowCount()].push_back(refGroup);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		//Add Group tab
		orderTab->addTab(groupTab, "G_" + QString::number(i));

		referralsWindow.tabWidget->addTab(orderTab, QString::number(orderTableModel->rowCount() + 1));
		referralsWindow.tabWidget->setTabEnabled(referralsWindow.tabWidget->count(), true);
		//==> Add Group tab
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		usedAccounts = new QStandardItem(accountsStr);
		emailsItem = new QStandardItem(emailsStr);
	}
	//==> Add new entry to QTableView
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//Add it here!
	QList<QStandardItem*> row;

	refferalLink->setBackground(QColor(255, 255, 255));
	finishedCountItem->setBackground(QColor(255, 255, 255));
	refferalCount->setBackground(QColor(255, 255, 255));
	usedAccounts->setBackground(QColor(255, 255, 255));
	status->setBackground(QColor(255, 255, 255));
	typeColumn->setBackground(QColor(255, 255, 255));
	addedFinishedItem->setBackground(QColor(255, 255, 255));
	finishedCountItem->setBackground(QColor(255, 255, 255));
	platform->setBackground(QColor(255, 255, 255));
	emailsItem->setBackground(QColor(255, 255, 255));

	row << refferalLink << refferalCount << usedAccounts << status << typeColumn << addedFinishedItem << finishedCountItem << platform << emailsItem;
	orderTableModel->appendRow(row);

	//Rewrite EMails.txt
	std::vector<QString> remainingLines;

	while (!emailStream.atEnd())
	{
		remainingLines.push_back(emailStream.readLine(0) + "\n");
	}

	//Delete content of EMails.txt
	emails.resize(0);

	//Rewrite EMails.txt
	for (QString line : remainingLines)
		emails.write(line.toUtf8());

	//Save table to database
	saveOrderDb();
	sendOrders(All);

	addOrderDialog.addOrderButton->setEnabled(true);
	addOrderDialog.addOrderButton->parentWidget()->close();
}

void ControlPanel::onAddOrderPopUpQueueButtonPressed()
{
	if (addQueueOrderDialog.refferalCountEditBox->text().isEmpty())
	{
		QMessageBox::warning(this, "Error!", "Referral Count is empty!");
		return;
	}

	if (addQueueOrderDialog.refferalLinkEditBox->text().isEmpty())
	{
		QMessageBox::warning(this, "Error!", "Referral Link is empty!");
		return;
	}

	if (!addQueueOrderDialog.level5RadioButton->isChecked() && !addQueueOrderDialog.level10RadioButton->isChecked() && !addQueueOrderDialog.level30RadioButton->isChecked())
	{
		QMessageBox::warning(this, "Error!", "Please choose a type!");
		return;
	}

	if (!addQueueOrderDialog.euwRadioButton->isChecked() && !addQueueOrderDialog.euneRadioButton->isChecked())
	{
		QMessageBox::warning(this, "Error!", "Please choose a platform!");
		return;
	}

	//Grab user input
	QStandardItem *refferalCount = new QStandardItem(addQueueOrderDialog.refferalCountEditBox->text());
	QStandardItem *refferalLink = new QStandardItem(addQueueOrderDialog.refferalLinkEditBox->text());
	QStandardItem *type = nullptr;
	QStandardItem *platform;

	if (addQueueOrderDialog.level5RadioButton->isChecked())
		type = new QStandardItem("LEVEL_5");

	if (addQueueOrderDialog.level10RadioButton->isChecked())
		type = new QStandardItem("LEVEL_10");

	if (addQueueOrderDialog.level30RadioButton->isChecked())
		type = new QStandardItem("LEVEL_30");

	if (addQueueOrderDialog.euwRadioButton->isChecked())
		platform = new QStandardItem("EUW");

	if (addQueueOrderDialog.euneRadioButton->isChecked())
		platform = new QStandardItem("EUNE");

	//Add it to the queue list here
	QList<QStandardItem*> row;

	refferalLink->setBackground(QColor(255, 255, 255));
	refferalCount->setBackground(QColor(255, 255, 255));
	type->setBackground(QColor(255, 255, 255));
	platform->setBackground(QColor(255, 255, 255));

	row << refferalLink << refferalCount << type << platform;

	queueTableModel->appendRow(row);
	saveQueueDb();

	sendOrders(All);
	addQueueOrderDialog.addOrderButton->parentWidget()->close();
}

void ControlPanel::addOrder(QString refID, uint16_t refCount, QString type, QString platform)
{
	//Create invisble addOrder Dialog
	QDialog dialog(this, Qt::Dialog);
	addOrderDialog.setupUi(&dialog);

	//Set queue order information
	addOrderDialog.refferalCountEditBox->setText(QString::number(refCount));
	addOrderDialog.refferalLinkEditBox->setText(refID);

	if (type == "LEVEL_5")
		addOrderDialog.level5RadioButton->setChecked(true);

	if (type == "LEVEL_10")
		addOrderDialog.level10RadioButton->setChecked(true);

	if (type == "LEVEL_30")
		addOrderDialog.level30RadioButton->setChecked(true);

	if (platform == "EUW")
		addOrderDialog.euwRadioButton->setChecked(true);

	if (platform == "EUNE")
		addOrderDialog.euneRadioButton->setChecked(true);

	//Now go!
	onAddOrderPopUpAddOrderButtonPressed();
	startOrder();

	saveOrderDb();
	sendClientOptions();
}

void ControlPanel::addQueueOrder(QString refID, uint16_t refCount, QString type, QString platform)
{
	//Grab user input
	QStandardItem *refferalCount = new QStandardItem(QString::number(refCount));
	QStandardItem *refferalLink = new QStandardItem(refID);
	QStandardItem *typeItem = new QStandardItem(type);
	QStandardItem *platformItem = new QStandardItem(platform);

	//Add it to the queue list here
	QList<QStandardItem*> row;

	refferalLink->setBackground(QColor(255, 255, 255));
	refferalCount->setBackground(QColor(255, 255, 255));
	typeItem->setBackground(QColor(255, 255, 255));
	platformItem->setBackground(QColor(255, 255, 255));

	row << refferalLink << refferalCount << typeItem << platformItem;

	queueTableModel->appendRow(row);
	saveQueueDb();

	sendOrders(All);
}

void ControlPanel::doQueueOrder()
{
	//Create invisble addOrder Dialog
	QDialog dialog(this, Qt::Dialog);
	addOrderDialog.setupUi(&dialog);

	//Get queue order information
	QString refLink = queueTableModel->data(queueTableModel->index(0, 0), Qt::EditRole).toString();
	int refCount = queueTableModel->data(queueTableModel->index(0, 1), Qt::EditRole).toInt();
	QString type = queueTableModel->data(queueTableModel->index(0, 2), Qt::EditRole).toString();
	QString platform = queueTableModel->data(queueTableModel->index(0, 3), Qt::EditRole).toString();

	//Set queue order information
	addOrderDialog.refferalCountEditBox->setText(QString::number(refCount));
	addOrderDialog.refferalLinkEditBox->setText(refLink);

	if (type == "LEVEL_5")
		addOrderDialog.level5RadioButton->setChecked(true);

	if (type == "LEVEL_10")
		addOrderDialog.level10RadioButton->setChecked(true);

	if (type == "LEVEL_30")
		addOrderDialog.level30RadioButton->setChecked(true);

	if (platform == "EUW")
		addOrderDialog.euwRadioButton->setChecked(true);

	if (platform == "EUNE")
		addOrderDialog.euneRadioButton->setChecked(true);

	//Now go!
	onAddOrderPopUpAddOrderButtonPressed();

	//Start Order
	startOrder();

	//Remove order from queue
	queueTableModel->removeRow(0);

	//Save all this!
	saveQueueDb();
	saveOrderDb();
}

void ControlPanel::startOrder()
{
	ui.startOrderButton->setEnabled(false);

	int orderLimit = queueWindow.orderLimitEdit->text().toInt();
	int row = orderTableModel->rowCount() - 1;

	if (orderLimit)
	{
		if (activeOrders >= orderLimit)
		{
			QMessageBox::warning(this, "Error!", "Order limit reached!");
			ui.startOrderButton->setEnabled(true);
			return;
		}
	}

	if (orderTableModel->data(orderTableModel->index(row, 3), Qt::EditRole) == "FINISHED" || orderTableModel->data(orderTableModel->index(row, 3), Qt::EditRole) == "STARTED")
	{
		QMessageBox::warning(this, "Error!", "Order Type seems wrong!");
		return;
	}

	//Progressbar stuff
	int startedClients = 0;
	int clientCount = m_orders[row].size() * 10;

	QProgressDialog startClients("Starting Clients... (0/" + QString::number(clientCount) + ")", "Abort", 0, clientCount, (QWidget*)parent());
	startClients.show();

	for (auto refGroup : m_orders[row])
	{
		for (int i = 0; i < 10; ++i)
		{
			//Start Client here!
			qint64 processId = 0;
			QProcess *pvpProcess = new QProcess(this);
			pvpProcess->startDetached("PvPNetClient.exe", refGroup->player[i].commandLine, "", &processId);

			//Some additional saving...
			refGroup->player[i].pvpClientHandle = OpenProcess(PROCESS_TERMINATE, false, processId);
			refGroup->player[i].processPtr = pvpProcess;

			//Search for Console Window!
			HWND consoleHwnd = NULL;

			while (!consoleHwnd)
			{
				consoleHwnd = FindWindowA("ConsoleWindowClass", refGroup->player[i].userName.toStdString().c_str());
			}

			//Move Console Window into our application!
			SetParent(consoleHwnd, refGroup->player[i].pvpLogWnd);
			SetWindowPos(consoleHwnd, 0, 0, 0, 0, 0, 0x0001 | 0x0040);
			ShowWindow(consoleHwnd, SW_MAXIMIZE);

			//Progressbar stuff again!
			++startedClients;
			startClients.setValue(startedClients);
			startClients.setLabelText("Starting Clients... (" + QString::number(startedClients) + "/" + QString::number(clientCount) + ")");

			//We still wanna react to events hm?
			qApp->processEvents();
		}
	}

	//Update status
	orderTableModel->setData(orderTableModel->index(row, 3), "STARTED", Qt::EditRole);
	orderTableModel->setData(orderTableModel->index(row, 3), QColor(0, 255, 0), Qt::BackgroundRole);

	//Start referral updater
	ReferralUpdater *referralUpdater = new ReferralUpdater(&referralsWindow, row, this);

	connect(referralUpdater, SIGNAL(addEmuConsole(HWND, HWND)), this, SLOT(onAddEmuConsole(HWND, HWND)));
	connect(referralUpdater, SIGNAL(addPvPConsole(HWND, HWND)), this, SLOT(onAddPvPConsole(HWND, HWND)));
	connect(referralUpdater, SIGNAL(orderFinished(int)), this, SLOT(onOrderFinished(int)));
	connect(referralUpdater, SIGNAL(groupFinished(int, int)), this, SLOT(onGroupFinished(int, int)));
	connect(referralUpdater, SIGNAL(userFinished(int, int, int)), this, SLOT(onUserFinished(int, int, int)));

	referralUpdater->start();

	++activeOrders;
	ui.startOrderButton->setEnabled(true);
}

void ControlPanel::onQueueActivedStateChanged(int stateChange)
{
	if (stateChange == Qt::Checked)
		queueActivated = true;
	else
		queueActivated = false;

	int orderLimit = queueWindow.orderLimitEdit->text().toInt();

	//Save in Settings
	appSettings->beginGroup("Main.Settings");
	appSettings->setValue("QueueActivated", queueActivated);
	appSettings->setValue("OrderLimit", orderLimit);
	appSettings->endGroup();

	sendClientOptions();

	if (activeOrders < orderLimit && queueTableModel->rowCount() && queueActivated)
	{
		int queueCount = orderLimit - activeOrders;

		if (queueCount >= queueTableModel->rowCount())
			queueCount = queueTableModel->rowCount();

		while (queueCount)
		{
			doQueueOrder();
			--queueCount;
		}
	}
}

void ControlPanel::onStartOrderButtonPressed()
{
	ui.startOrderButton->setEnabled(false);

	int orderLimit = queueWindow.orderLimitEdit->text().toInt();

	if (orderLimit)
	{
		if (activeOrders >= orderLimit)
		{
			QMessageBox::warning(this, "Error!", "Order limit reached!");
			ui.startOrderButton->setEnabled(true);
			return;
		}
	}

	auto currentIndex = ui.orderTable->selectionModel()->currentIndex();

	if (currentIndex.isValid())
	{
		if (orderTableModel->data(orderTableModel->index(currentIndex.row(), 3), Qt::EditRole) == "FINISHED" || orderTableModel->data(orderTableModel->index(currentIndex.row(), 3), Qt::EditRole) == "STARTED")
			return;

		//Progressbar stuff
		int startedClients = 0;
		int clientCount = m_orders[currentIndex.row()].size() * 10;

		QProgressDialog startClients("Starting Clients... (0/" + QString::number(clientCount) + ")", "Abort", 0, clientCount, (QWidget*)parent());
		startClients.show();

		for (auto refGroup : m_orders[currentIndex.row()])
		{
			for (int i = 0; i < 10; ++i)
			{
				//Start Client here!
				qint64 processId = 0;
				QProcess *pvpProcess = new QProcess(this);
				pvpProcess->startDetached("PvPNetClient.exe", refGroup->player[i].commandLine, "", &processId);

				//Some additional saving...
				refGroup->player[i].pvpClientHandle = OpenProcess(PROCESS_TERMINATE, false, processId);
				refGroup->player[i].processPtr = pvpProcess;

				//Search for Console Window!
				HWND consoleHwnd = NULL;

				while (!consoleHwnd)
				{
					consoleHwnd = FindWindowA("ConsoleWindowClass", refGroup->player[i].userName.toStdString().c_str());
				}

				//Move Console Window into our application!
				SetParent(consoleHwnd, refGroup->player[i].pvpLogWnd);
				SetWindowPos(consoleHwnd, 0, 0, 0, 0, 0, 0x0001 | 0x0040);
				ShowWindow(consoleHwnd, SW_MAXIMIZE);

				//Progressbar stuff again!
				++startedClients;
				startClients.setValue(startedClients);
				startClients.setLabelText("Starting Clients... (" + QString::number(startedClients) + "/" + QString::number(clientCount) + ")");

				//We still wanna react to events hm?
				qApp->processEvents();
			}
		}

		//Update status
		orderTableModel->setData(orderTableModel->index(currentIndex.row(), 3), "STARTED", Qt::EditRole);
		orderTableModel->setData(orderTableModel->index(currentIndex.row(), 3), QColor(0, 255, 0), Qt::BackgroundRole);

		//Start referral updater
		ReferralUpdater *referralUpdater = new ReferralUpdater(&referralsWindow, currentIndex.row(), this);

		connect(referralUpdater, SIGNAL(addEmuConsole(HWND, HWND)), this, SLOT(onAddEmuConsole(HWND, HWND)));
		connect(referralUpdater, SIGNAL(addPvPConsole(HWND, HWND)), this, SLOT(onAddPvPConsole(HWND, HWND)));
		connect(referralUpdater, SIGNAL(orderFinished(int)), this, SLOT(onOrderFinished(int)));
		connect(referralUpdater, SIGNAL(groupFinished(int, int)), this, SLOT(onGroupFinished(int, int)));
		connect(referralUpdater, SIGNAL(userFinished(int, int, int)), this, SLOT(onUserFinished(int, int, int)));

		referralUpdater->start();
	}
	else
	{
		QMessageBox::warning(this, "Error!", "currentIndex() - Invalid!");
	}

	++activeOrders;
	sendClientOptions();
	ui.startOrderButton->setEnabled(true);
}

void ControlPanel::onResumeOrderButtonPressed()
{
}

void ControlPanel::onStopOrderButtonPressed()
{
	QMessageBox sureQuestion;
	sureQuestion.setWindowTitle("Are you sure?");
	sureQuestion.setText("Do you really want to stop the order?");
	sureQuestion.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

	if (sureQuestion.exec() != QMessageBox::Yes)
		return;

	auto currentIndex = ui.orderTable->selectionModel()->currentIndex();

	if (currentIndex.isValid())
	{
		if (orderTableModel->data(orderTableModel->index(currentIndex.row(), 3), Qt::EditRole) == "FINISHED" || orderTableModel->data(orderTableModel->index(currentIndex.row(), 3), Qt::EditRole) == "PAUSED")
			return;

		--activeOrders;

		/*if (m_orders[currentIndex.row()].size())
		{*/
		for (auto refGroup : m_orders[currentIndex.row()])
		{
			for (int i = 0; i < 10; ++i)
			{
				delete refGroup->player[i].processPtr;
				TerminateProcess(refGroup->player[i].pvpClientHandle, 0);
			}
		}
		/*}*/

		//Update status
		orderTableModel->setData(orderTableModel->index(currentIndex.row(), 3), "PAUSED", Qt::EditRole);
		orderTableModel->setData(orderTableModel->index(currentIndex.row(), 3), QColor(255, 255, 255), Qt::BackgroundRole);

		//Finished Count => 0
		orderTableModel->setData(orderTableModel->index(currentIndex.row(), 6), "0", Qt::EditRole);

		//if (statusItem)
		//	statusItem->setData("STARTED");
	}
}

void ControlPanel::onAddEmuConsole(HWND emuHwnd, HWND parentHwnd)
{
	SetParent(emuHwnd, parentHwnd);
	SetWindowPos(emuHwnd, 0, 0, 0, 0, 300, 0x01 | 0x0040);
	ShowWindow(emuHwnd, SW_MAXIMIZE);

	HANDLE processHandle = (HANDLE)1;

	while (processHandle)
	{
		processHandle = GetProcessByName(L"WerFault.exe");
		TerminateProcess(processHandle, 0);
	}
}

void ControlPanel::onAddPvPConsole(HWND pvpHwnd, HWND parentHwnd)
{
	SetParent(pvpHwnd, parentHwnd);
	SetWindowPos(pvpHwnd, 0, 0, 0, 0, 300, 0x01 | 0x0040);
	ShowWindow(pvpHwnd, SW_MAXIMIZE);
}

void ControlPanel::onOrderFinished(int orderId)
{
	if (referralsWindow.tabWidget->isTabEnabled(orderId))
	{
		//Update status
		QString tabName = referralsWindow.tabWidget->tabText(orderId);

		orderTableModel->setData(orderTableModel->index(tabName.toInt() - 1, 3), "FINISHED", Qt::EditRole);
		orderTableModel->setData(orderTableModel->index(tabName.toInt() - 1, 3), QColor(255, 165, 0), Qt::BackgroundRole);

		//Exit all PvPClient instances
		for (auto refGroup : m_orders[orderId])
		{
			for (int i = 0; i < 10; ++i)
			{
				if (refGroup->player[i].processPtr != nullptr)
					delete refGroup->player[i].processPtr;

				TerminateProcess(refGroup->player[i].pvpClientHandle, 0);
			}
		}

		//Cleanup m_orders
		for (auto refGroup : m_orders[orderId])
		{
			if (refGroup != nullptr)
				delete refGroup;
		}

		m_orders.erase(orderId);

		//Update Finished Time
		auto addedFinishedItem = orderTableModel->index(tabName.toInt() - 1, 5);
		orderTableModel->setData(addedFinishedItem, addedFinishedItem.data(Qt::EditRole).toString() + "-" + QDateTime::currentDateTime().toString(), Qt::EditRole);

		//Save it!
		saveOrderDb();

		//Send to Master Server
		sendOrders(All);

		//Disable Order Tab
		referralsWindow.tabWidget->setTabEnabled(orderId, false);

		//Queue work here!
		--activeOrders;
		int orderLimit = queueWindow.orderLimitEdit->text().toInt();

		if (activeOrders < orderLimit && queueTableModel->rowCount() && queueActivated)
		{
			int queueCount = orderLimit - activeOrders;

			if (queueCount >= queueTableModel->rowCount())
				queueCount = queueTableModel->rowCount();

			while (queueCount)
			{
				doQueueOrder();
				--queueCount;
			}
		}
	}
}

void ControlPanel::onGroupFinished(int orderId, int groupId)
{
	//Cleanup tabs? TODO
	auto groupTab = (QTabWidget*)referralsWindow.tabWidget->widget(orderId);

	if (groupTab->isTabEnabled(groupId))
	{
		QString orderTabName = referralsWindow.tabWidget->tabText(orderId);

		groupTab->setTabEnabled(groupId, false);

		//Exit all group PvPClient instances
		auto refGroups = m_orders[orderTabName.toInt() - 1];

		if (refGroups.empty())
			return;

		auto refGroup = refGroups[groupId];

		if (refGroup == nullptr)
			return;

		for (int i = 0; i < 10; ++i)
		{
			TerminateProcess(refGroup->player[i].pvpClientHandle, 0);
		}

		//Create file that this hosts group has finished
		QFile("Logs/" + refGroup->player[0].userName + ".gfinished").open(QIODevice::WriteOnly);

		/*auto finishedCountItem = orderTableModel->index(orderTabName.toInt() - 1, 6);
		orderTableModel->setData(finishedCountItem, QString::number(finishedCountItem.data(Qt::EditRole).toString().toInt() + 10), Qt::EditRole);*/
	}
}

void ControlPanel::onUserFinished(int orderId, int groupId, int userId)
{
	//Cleanup tabs? TODO
	auto groupTab = (QTabWidget*)referralsWindow.tabWidget->widget(orderId);
	auto userTab = (QTabWidget*)groupTab->widget(groupId);

	if (userTab->isTabEnabled(userId))
	{
		QString orderTabName = referralsWindow.tabWidget->tabText(orderId);

		userTab->setTabEnabled(userId, false);

		auto finishedCountItem = orderTableModel->index(orderTabName.toInt() - 1, 6);
		orderTableModel->setData(finishedCountItem, QString::number(finishedCountItem.data(Qt::EditRole).toString().toInt() + 1), Qt::EditRole);
	}
}

void ControlPanel::connected()
{
	ui.statusBar->showMessage("Connected!");

	connect(masterServerSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(masterServerSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

	sendClientOptions();
}

void ControlPanel::disconnected()
{
	ui.statusBar->showMessage("Disconnected!");
}

void ControlPanel::readyRead()
{
	QTcpSocket *socket = (QTcpSocket*)sender();

	if (socket == NULL)
		return;

	if (socket->state() != QAbstractSocket::ConnectedState)
		return;

	if (socket->bytesAvailable() >= sizeof(PacketHeader))
	{
		//Check header
		PacketHeader *header = (PacketHeader*)socket->peek(sizeof(PacketHeader)).data();

		if (header->magic != MAGIC)
		{
			debugWindow.debugTextEdit->append("Packet received from -> Invalid magic");
			socket->disconnectFromHost();
			return;
		}

		//Retrieve full packet
		qint16 bodySize = header->bodySize;

		if (socket->bytesAvailable() < bodySize + sizeof(PacketHeader))
			return;

		header = (PacketHeader*)socket->read(sizeof(PacketHeader)).data();
		QByteArray packetBody = socket->read(bodySize);

		//Handle packet
		if (header->packetId >= PacketID_MAX)
		{
			debugWindow.debugTextEdit->append("Received packet with packetID >= PacketID_MAX");
			socket->disconnectFromHost();
			return;
		}

		//Look for handler
		packetHandlerType packetHandler = m_handlerTable[header->packetId];

		if (packetHandler)
		{
			if (!(*this.*packetHandler)(packetBody))
			{
				debugWindow.debugTextEdit->append("Handler could not handle packet " + QString::number((qint16)header->packetId));
				socket->disconnectFromHost();
			}
		}
		else
		{
			debugWindow.debugTextEdit->append("Received packet with no available handler = " + QString::number((qint16)header->packetId));
			socket->disconnectFromHost();
			return;
		}
	}
}

void ControlPanel::registerHandler(packetHandlerType handler, PacketID packetId)
{
	m_handlerTable[packetId] = handler;
}

void ControlPanel::sendPacket(QByteArray packet)
{
	int dataToSend = packet.size();
	int sentData = 0;

	char *sendBuffer = packet.data();

	while (dataToSend > 0)
	{
		sentData = masterServerSocket->write(sendBuffer, dataToSend);

		if (sentData == -1)
			return;

		sendBuffer += sentData;
		dataToSend -= sentData;
	}

	//masterServerSocket->flush();
}

void ControlPanel::sendClientOptions()
{
	if (masterServerSocket == nullptr)
		return;

	if (masterServerSocket->state() != QAbstractSocket::ConnectedState)
		return;

	ULONGLONG ramInKB;

	if (!GetPhysicallyInstalledSystemMemory(&ramInKB))
	{
		debugWindow.debugTextEdit->append("Could not retrieve RAM : " + QString::number(GetLastError()));
		ui.statusBar->showMessage("FATAL ERROR => DebugWindow");
		return;
	}

	//Prepare and send ClientOptions
	SClientOptions clientOptions;
	clientOptions.setVersion("V0.0.1");
	clientOptions.setCpName(ui.cpNameLineEdit->text());
	clientOptions.setMaxRam(ramInKB / 1000);
	clientOptions.setQueueActivated(queueActivated);

	int orderLimit = queueWindow.orderLimitEdit->text().toInt();
	clientOptions.setOrderLimit(orderLimit);

	QByteArray sendData = clientOptions.parse();

	debugWindow.debugTextEdit->append("ClientOptions {" + QString::number(ramInKB / 1000) + "MB} - " + QString::number(sendData.size()));

	sendPacket(sendData);
}

void ControlPanel::sendOrders(OrderType filter)
{
	if (masterServerSocket == nullptr)
		return;

	if (masterServerSocket->state() != QAbstractSocket::ConnectedState)
		return;

	//Loop through orderTable
	SOrdersPacket ordersPacket;

	if (filter == All || filter == Queue)
	{
		//QueueTable
		for (int i = 0; i < queueTableModel->rowCount(); ++i)
		{
			Order *order = new Order;

			//RefID
			QString refID = queueTableModel->data(queueTableModel->index(i, 0), Qt::EditRole).toString();
			strcpy(order->refID, refID.toStdString().c_str());

			//Ref Count
			order->refCount = queueTableModel->data(queueTableModel->index(i, 1), Qt::EditRole).toInt();

			//Order Type
			order->orderType = Queue;

			//Get Added & Finished Time
			//NOT AVAILABLE

			//Finished Count
			//NOT AVAILABLE
			order->finishedCount = 0;

			//LevelCap
			QString levelCap = queueTableModel->data(queueTableModel->index(i, 2), Qt::EditRole).toString();
			strcpy(order->levelCap, levelCap.toStdString().c_str());

			//Platform
			QString platform = queueTableModel->data(queueTableModel->index(i, 3), Qt::EditRole).toString();
			memcpy(order->platform, platform.toStdString().c_str(), platform.length());

			//Control Panel Name

			strcpy(order->cpName, ui.cpNameLineEdit->text().toStdString().c_str());
			ordersPacket.addOrder(order);
		}
	}

	//OrderTable
	for (int i = 0; i < orderTableModel->rowCount(); ++i)
	{
		if ((filter == Finished && orderTableModel->data(orderTableModel->index(i, 3), Qt::EditRole) == "FINISHED")
			|| (filter == Paused && orderTableModel->data(orderTableModel->index(i, 3), Qt::EditRole) == "PAUSED")
			|| (filter == Started && orderTableModel->data(orderTableModel->index(i, 3), Qt::EditRole) == "STARTED")
			|| filter == All)
		{
			Order *order = new Order;

			//RefID
			QString refID = orderTableModel->data(orderTableModel->index(i, 0), Qt::EditRole).toString();
			strcpy(order->refID, refID.toStdString().c_str());

			//Ref Count
			order->refCount = orderTableModel->data(orderTableModel->index(i, 1), Qt::EditRole).toInt();

			//Order Type
			QString orderType = orderTableModel->data(orderTableModel->index(i, 3), Qt::EditRole).toString();

			if (orderType == "FINISHED")
				order->orderType = Finished;
			else if (orderType == "PAUSED")
				order->orderType = Paused;
			else if (orderType == "STARTED")
				order->orderType = Started;

			//LevelCap
			QString levelCap = orderTableModel->data(orderTableModel->index(i, 4), Qt::EditRole).toString();
			strcpy(order->levelCap, levelCap.toStdString().c_str());

			//Get Added & Finished Time
			std::vector<std::string> dateVector = split(orderTableModel->data(orderTableModel->index(i, 5), Qt::EditRole).toString().toStdString().c_str(), '-');

			if (dateVector.size() >= 1)
			{
				memcpy(order->startedTime, &QDateTime::fromString(QString::fromUtf8(dateVector.at(0).c_str()), Qt::SystemLocaleShortDate), sizeof(order->startedTime));

				if (dateVector.size() == 2)
					memcpy(order->finishedTime, &QDateTime::fromString(QString::fromUtf8(dateVector.at(1).c_str()), Qt::SystemLocaleShortDate), sizeof(order->finishedTime));
			}

			//Finished Count
			order->finishedCount = orderTableModel->data(orderTableModel->index(i, 6), Qt::EditRole).toInt();

			//Platform
			QString platform = orderTableModel->data(orderTableModel->index(i, 7), Qt::EditRole).toString();
			memcpy(order->platform, platform.toStdString().c_str(), platform.length());

			//Control Panel Name
			strcpy(order->cpName, ui.cpNameLineEdit->text().toStdString().c_str());

			ordersPacket.addOrder(order);
		}
	}

	sendPacket(ordersPacket.parse());

	debugWindow.debugTextEdit->append("OrdersPacket sent!");
}

//Packet handler here below!
bool ControlPanel::clientOptionsHandler(QByteArray &packet)
{
	RClientOptions clientOptionsPacket(packet);

	debugWindow.debugTextEdit->append("ClientOptions received! {" +
		clientOptionsPacket.getCpName() + ", " +
		QString::number(clientOptionsPacket.getOrderLimit()) + ", " +
		QString::number(clientOptionsPacket.isQueueActivated()) + "}");

	ui.cpNameLineEdit->setText(clientOptionsPacket.getCpName());
	queueWindow.orderLimitEdit->setText(QString::number(clientOptionsPacket.getOrderLimit()));
	queueWindow.queueActivatedCheckBox->setChecked(clientOptionsPacket.isQueueActivated());

	return true;
}

bool ControlPanel::getOrdersHandler(QByteArray &packet)
{
	RGetOrders getOrdersPacket(packet);
	debugWindow.debugTextEdit->append("GetOrders received! {" + QString::number(getOrdersPacket.getOrderFilter()) + "}");

	sendOrders(getOrdersPacket.getOrderFilter());

	return true;
}

bool ControlPanel::addOrderHandler(QByteArray &packet)
{
	RAddOrder addOrderPacket(packet);

	debugWindow.debugTextEdit->append("AddOrderPacket received! {" +
		addOrderPacket.getRefID() + ", " +
		QString::number(addOrderPacket.getRefCount()) + ", " +
		QString::number(addOrderPacket.getOrderType()) + ", " +
		addOrderPacket.getPlatform() + ", " +
		addOrderPacket.getLevelCap() + "}");

	if (addOrderPacket.getOrderType() == Queue)
		addQueueOrder(addOrderPacket.getRefID(), addOrderPacket.getRefCount(), addOrderPacket.getLevelCap(), addOrderPacket.getPlatform());
	else if (addOrderPacket.getOrderType() == NormalOrder)
		addOrder(addOrderPacket.getRefID(), addOrderPacket.getRefCount(), addOrderPacket.getLevelCap(), addOrderPacket.getPlatform());

	return true;
}

#include "ControlPanel.moc"