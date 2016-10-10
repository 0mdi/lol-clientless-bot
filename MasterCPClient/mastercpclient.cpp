#include "mastercpclient.h"

MasterCPClient::MasterCPClient(QWidget *parent)
	: QMainWindow(parent)
{
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//Window setUp
	ui.setupUi(this);
	debugWindow.setupUi(&debugDialog);
	addOrderWindow.setupUi(&addOrderDialog);
	//==> Window setUp

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//Signals & Slots
	connect(ui.actionDebug, SIGNAL(triggered()), this, SLOT(onDebugAction()));

	connect(ui.addOrderButton, SIGNAL(pressed()), this, SLOT(onAddOrderPressed()));
	//==> Signals & Slots

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//Load Table Headers
	orderTableModel = new QStandardItemModel();
	orderTableModel->setRowCount(0);

	orderTableModel->setHorizontalHeaderItem(0, new QStandardItem(QString(QString::fromUtf8("ControlPanel"))));
	orderTableModel->setHorizontalHeaderItem(1, new QStandardItem(QString(QString::fromUtf8("Ref ID"))));
	orderTableModel->setHorizontalHeaderItem(2, new QStandardItem(QString(QString::fromUtf8("Ref Count"))));
	orderTableModel->setHorizontalHeaderItem(3, new QStandardItem(QString(QString::fromUtf8("Status"))));
	orderTableModel->setHorizontalHeaderItem(4, new QStandardItem(QString(QString::fromUtf8("Type"))));
	orderTableModel->setHorizontalHeaderItem(5, new QStandardItem(QString(QString::fromUtf8("Added - Finished"))));
	orderTableModel->setHorizontalHeaderItem(6, new QStandardItem(QString(QString::fromUtf8("Finished Count"))));
	orderTableModel->setHorizontalHeaderItem(7, new QStandardItem(QString(QString::fromUtf8("Platform"))));

	ui.orderTable->setModel(orderTableModel);
	//==> Load Table Headers

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//Load Settings
	appSettings = new QSettings("master_config.ini", QSettings::IniFormat);

	if (!QFile::exists("master_config.ini"))
	{
		appSettings->beginGroup("MasterCP.Settings");
		appSettings->setValue("IP", "127.0.0.1");
		appSettings->setValue("PORT", 1338);
		appSettings->endGroup();
	}
	//==> Load Settings

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	//Register Handlers
	registerHandler(&MasterCPClient::ordersPacketHandler, OrdersPacket);
	//==> Register Handlers

	//Connect to Server
	appSettings->beginGroup("MasterCP.Settings");
	QString serverIP = appSettings->value("IP").toString();
	qint16 serverPort = appSettings->value("PORT").toInt();
	appSettings->endGroup();

	debugWindow.debugTextEdit->append("Connecting to Master Server (" + serverIP + ", " + QString::number(serverPort) + ")");
	ui.statusBar->showMessage("Connecting to Master Server (" + serverIP + ", " + QString::number(serverPort) + ")");

	masterServerSocket = new QTcpSocket();

	connect(masterServerSocket, SIGNAL(connected()), this, SLOT(connected()));
	masterServerSocket->connectToHost(serverIP, serverPort);
	//==> Connect to Server

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}

MasterCPClient::~MasterCPClient()
{
	delete appSettings;
}

//Private Functions here
void MasterCPClient::sendPacket(QByteArray packet)
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

void MasterCPClient::registerHandler(packetHandlerType handler, PacketID packetId)
{
	m_handlerTable[packetId] = handler;
}

//Public Functions here

//Slots here pl0x
void MasterCPClient::connected()
{
	debugWindow.debugTextEdit->append("Connected!");
	ui.statusBar->showMessage("Connected!");

	connect(masterServerSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(masterServerSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

	//Send GetOrders Packet
	SGetOrders getOrdersPacket(All);
	sendPacket(getOrdersPacket.parse());

	debugWindow.debugTextEdit->append("GetOrders sent! {All}");
}

void MasterCPClient::disconnected()
{
	debugWindow.debugTextEdit->append("Disconnected!");
}

void MasterCPClient::readyRead()
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

void MasterCPClient::onDebugAction()
{
	debugDialog.exec();
}

void MasterCPClient::onAddOrderPressed()
{
	connect(addOrderWindow.addOrderButton, SIGNAL(pressed()), this, SLOT(onCreateOrderPressed()));
	addOrderDialog.exec();
}

void MasterCPClient::onCreateOrderPressed()
{
	if (addOrderWindow.refferalCountEditBox->text().isEmpty())
	{
		QMessageBox::warning(this, "Error!", "Referral Count is empty!");
		return;
	}

	if (addOrderWindow.refferalLinkEditBox->text().isEmpty())
	{
		QMessageBox::warning(this, "Error!", "Referral Link is empty!");
		return;
	}

	if (!addOrderWindow.level5RadioButton->isChecked() && !addOrderWindow.level10RadioButton->isChecked() && !addOrderWindow.level30RadioButton->isChecked())
	{
		QMessageBox::warning(this, "Error!", "Please choose a type!");
		return;
	}

	if (!addOrderWindow.euwRadioButton->isChecked() && !addOrderWindow.euneRadioButton->isChecked())
	{
		QMessageBox::warning(this, "Error!", "Please choose a platform!");
		return;
	}

	addOrderWindow.addOrderButton->setEnabled(false);

	//Grab user input
	qint16 refCount = addOrderWindow.refferalCountEditBox->text().toInt();
	QString refID = addOrderWindow.refferalLinkEditBox->text();
	QString levelCap;
	QString platform;

	if (addOrderWindow.level5RadioButton->isChecked())
		levelCap = "LEVEL_5";

	if (addOrderWindow.level10RadioButton->isChecked())
		levelCap = "LEVEL_10";

	if (addOrderWindow.level30RadioButton->isChecked())
		levelCap = "LEVEL_30";

	if (addOrderWindow.euwRadioButton->isChecked())
		platform = "EUW";

	if (addOrderWindow.euneRadioButton->isChecked())
		platform = "EUNE";

	SAddOrder addOrderPacket(refID, refCount, NormalOrder, platform, levelCap);
	sendPacket(addOrderPacket.parse());

	debugWindow.debugTextEdit->append("AddOrderPacket sent!");

	addOrderDialog.close();
	addOrderWindow.addOrderButton->setEnabled(true);
}

//And finally Handlers here
bool MasterCPClient::ordersPacketHandler(QByteArray &packet)
{
	ROrdersPacket ordersPacket(packet);

	debugWindow.debugTextEdit->append("OrderPacket received! {" + QString::number(ordersPacket.getOrders().count()) + "}");

	//Clear current orderTable
	orderTableModel->invisibleRootItem()->removeRows(0, orderTableModel->rowCount());

	//Iterate through orders and add them
	for (int i = 0; i < ordersPacket.getOrders().count(); ++i)
	{
		Order *order = ordersPacket.getOrders().at(i);

		QList<QStandardItem*> row;

		QStandardItem *cpName = new QStandardItem(order->cpName);
		QStandardItem *refID = new QStandardItem(order->refID);
		QStandardItem *refCount = new QStandardItem(QString::number(order->refCount));
		QStandardItem *type = nullptr;
		QStandardItem *levelCap = new QStandardItem(order->levelCap);
		QStandardItem *addedFinished = new QStandardItem(); //TODO: IMPLEMENT THIS!
		QStandardItem *finishedCount = new QStandardItem(QString::number(order->finishedCount));
		QStandardItem *platform = new QStandardItem(order->platform);

		switch (order->orderType)
		{
		case Paused:
			type = new QStandardItem("PAUSED");
			break;

		case Started:
			type = new QStandardItem("STARTED");
			type->setBackground(QColor(0, 255, 0));
			break;

		case Finished:
			type = new QStandardItem("FINISHED");
			type->setBackground(QColor(255, 165, 0));
			break;

		case Queue:
			type = new QStandardItem("QUEUE");
			type->setBackground(QColor(148, 0, 211));
			break;
		}

		row << cpName << refID << refCount << type << levelCap << addedFinished << finishedCount << platform;
		orderTableModel->appendRow(row);
	}

	return true;
}