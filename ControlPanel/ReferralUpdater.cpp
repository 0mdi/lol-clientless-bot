#include "ControlPanel.h"
#include "ReferralUpdater.h"

#include <TlHelp32.h>

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

ReferralUpdater::ReferralUpdater(Ui_referralWindow *object, int orderNumber, QObject *parent)
	: QThread(parent)
	, m_object(object)
	, m_orderNumber(orderNumber)
{
	m_cpParent = (ControlPanel*)parent;
}

ReferralUpdater::~ReferralUpdater()
{
}

void ReferralUpdater::run()
{
	int werFaultCounter = 0;

	QTabWidget *orderTab = nullptr;
	int tabID = -1;

	//Iterate through every orderTab and check if it matches the orderID
	for (int i = 0; i < m_object->tabWidget->count(); ++i)
	{
		//Ignore FINISHED or PAUSED orders
		if (!m_cpParent->isOrderRunning(m_object->tabWidget->tabText(i).toUInt() - 1))
			continue;

		//Does it match?
		if (m_object->tabWidget->tabText(i).toUInt() - 1 == m_orderNumber)
		{
			orderTab = (QTabWidget*)m_object->tabWidget->widget(i);
			tabID = i;
			break;
		}
	}

	//We didn't find the order!
	if (orderTab == nullptr || tabID == -1)
	{
		MessageBoxA(0, "Could not find corresponding tab to orderID", "Error!", 0);
		return;
	}

	//While the order is started, observe clients!
	while (m_cpParent->isOrderRunning(m_orderNumber))
	{
		//Order still enabled?
		if (!m_object->tabWidget->isTabEnabled(tabID))
			break;

		//Retrieve group information
		int groupCount = orderTab->count();
		int orderFinishedCount = 0;

		//Iterate through each group
		for (int i = 0; i < groupCount; ++i)
		{
			int groupFinishedCount = 0;

			//Is group enabled?
			if (!orderTab->isTabEnabled(i))
			{
				orderFinishedCount += 10;
				continue;
			}

			//Retrieve current group tab
			auto groupTab = (QTabWidget*)orderTab->widget(i);

			//Iterate through each user (default: 10)
			for (int j = 0; j < 10; ++j)
			{
				//Retrieve user tab
				auto userName = groupTab->tabText(j).toLower();

				//Check if user is already finished
				if (groupTab->isTabEnabled(j))
				{
					if (QFile::exists("Logs/" + userName + ".finished"))
					{
						emit userFinished(tabID, i, j);

						++groupFinishedCount;
						++orderFinishedCount;
					}
				}
				else
				{
					++groupFinishedCount;
					++orderFinishedCount;
				}

				//Search for top-level console
				HWND consoleWnd = FindWindowA("ConsoleWindowClass", (userName + "_c").toStdString().c_str());

				if (consoleWnd != NULL)
				{
					QTextEdit *textEdit = groupTab->widget(j)->findChild<QTextEdit*>("logEmuEdit");
					HWND textEditWnd = (HWND)textEdit->winId();

					emit addEmuConsole(consoleWnd, textEditWnd);
				}
			}

			//Check if group is already finished
			if (groupFinishedCount == 10)
			{
				emit groupFinished(tabID, i);
			}
		}

		//Check if order is finished
		if (orderFinishedCount >= groupCount * 10)
		{
			emit orderFinished(tabID);
		}

		msleep(2500);
	}

	//while (true)
	//{
	////OrdeTab
	//for (int i = 0; i < m_object->tabWidget->count(); ++i)
	//{
	//	//Check if order is started
	//	if (!m_cpParent->isOrderRunning(m_object->tabWidget->tabText(i).toUInt() - 1))
	//		continue;

	//	int finishedCount = 0;
	//	int groupCount = ((QTabWidget*)m_object->tabWidget->widget(i))->count();

	//	//GroupTab
	//	for (int j = 0; j < ((QTabWidget*)m_object->tabWidget->widget(i))->count(); ++j)
	//	{
	//		QTabWidget *orderTab = (QTabWidget*)m_object->tabWidget;
	//		QTabWidget *groupTab = (QTabWidget*)orderTab->widget(i);
	//		int groupFinishedCount = 0;

	//		for (int k = 0; k < ((QTabWidget*)groupTab->widget(j))->count(); ++k)
	//		{
	//			QTabWidget *tab = (QTabWidget*)((QTabWidget*)m_object->tabWidget->widget(i))->widget(j);
	//			QString name = tab->tabText(k).toLower();
	//
	//			//if (tab->isTabEnabled(k))
	//			//{
	//				if (QFile::exists("Logs/" + name + ".finished"))
	//				{
	//					emit userFinished(i, j, k);

	//					++finishedCount;
	//					++groupFinishedCount;
	//				}
	//			/*}*/

	//			//if (groupTab->isTabEnabled(j))
	//			//{
	//				if (groupFinishedCount == 10)
	//				{
	//					emit groupFinished(i, j);
	//				}
	//
	//			/*}*/

	//			//ClientEmu Log
	//			HWND consoleWnd = NULL;

	//			consoleWnd = FindWindowA("ConsoleWindowClass", (name + "_c").toStdString().c_str());

	//			if (consoleWnd != NULL)
	//			{
	//				QTextEdit *textEdit = tab->widget(k)->findChild<QTextEdit*>("logEmuEdit");
	//				HWND textEditWnd = (HWND)textEdit->winId();

	//				emit addEmuConsole(consoleWnd, textEditWnd);
	//			}

	//		}
	//	}

	//	if (finishedCount >= groupCount * 10)
	//	{
	//		emit orderFinished(i);
	//	}
	//

	//}

	//++werFaultCounter;

	//if (werFaultCounter == 5)
	//{
	//	//HANDLE processHandle = (HANDLE)1;

	//	//while (processHandle)
	//	//{
	//	//	processHandle = GetProcessByName(L"WerFault.exe");
	//	//	TerminateProcess(processHandle, 0);
	//	//}

	//	werFaultCounter = 0;
	//}

	//this->msleep(5000);

	//}
}

#include "ReferralUpdater.moc"