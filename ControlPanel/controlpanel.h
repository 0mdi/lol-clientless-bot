#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtCore/QtCore>
#include <QProcess>
#include <QStandardItemModel>
#include <QProgressDialog>
#include <QDataStream>
#include <QFile>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QDir>
#include <QLabel>
#include <QDateTime>
#include <QTime>
#include <QSettings>
#include <QtNetwork/QTcpSocket>

#include "ui_controlpanel.h"
#include "ui_aboutDialog.h"
#include "ui_addOrderDialog.h"
#include "ui_referralWindow.h"
#include "ui_queueWindow.h"
#include "ui_orderInfoDialog.h"
#include "ui_debugWindow.h"
#include "ReferralUpdater.h"
#include "protocol.h"

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <TlHelp32.h>
#include <Windows.h>

class ControlPanel;
typedef bool (ControlPanel::*packetHandlerType)(QByteArray&);

enum BotType
{
	UNKNOWN,
	LEVEL_5,
	LEVEL_10,
	LEVEL_30
};

struct Bot
{
	Bot()
	{
		pvpLogWnd = 0;
		pvpClientHandle = 0;
		curLevel = 0;
		expBalance = "0/0";
	}

	QString userName; // userName + "_c" == ClientEmu.exe ConsoleTitle
	QStringList commandLine; //PvPNetClient.exe
	HWND pvpLogWnd;
	HANDLE pvpClientHandle;
	BotType type;
	QProcess *processPtr;

	qint8 curLevel;
	QString expBalance;
	QString email; //Only for LVL_30
};

struct RefGroup
{
	/* 10 Player
	** player[0] = HOST
	*/
	Bot player[10];
};

class ControlPanel : public QMainWindow
{
	Q_OBJECT

public:
	ControlPanel(QWidget *parent = 0);
	~ControlPanel();

	void loadOrderEntries();
	void loadQueueOrderEntries();

	void closeEvent(QCloseEvent *event);

	bool isOrderRunning(unsigned int orderNumber);

private:
	void saveOrderDb();
	void saveQueueDb();

	void doQueueOrder();
	void startOrder();

	void addOrder(QString refID, uint16_t refCount, QString type, QString platform);
	void addQueueOrder(QString refID, uint16_t refCount, QString type, QString platform);

	void sendPacket(QByteArray packet);

	void sendClientOptions();
	void sendOrders(OrderType filter);

	void registerHandler(packetHandlerType, PacketID packetId);

	bool clientOptionsHandler(QByteArray &packet);
	bool getOrdersHandler(QByteArray &packet);
	bool addOrderHandler(QByteArray &packet);

	public slots:
	//Main Window
	void onCpNameEditingFinished();
	void onAboutActionTriggered();
	void onWindowReferalsActionTriggered();
	void onWindowQueueActionTriggered();
	void onWindowDebugActionTriggered();

	void onAddOrderButtonPressed();

	//Queue Window
	void onQueueActivedStateChanged(int);
	void onOrderLimitEditingFinished();

	//Context Menu
	void customMenuRequestedOrder(const QPoint&);

	//Button Slots
	void onAddOrderPopUpAddOrderButtonPressed();
	void onAddOrderQueueButtonPressed();
	void onAddOrderPopUpQueueButtonPressed();
	void onRemoveOrderButtonPressed();
	void onRemoveQueueOrderButtonPressed();
	void onStartOrderButtonPressed();
	void onResumeOrderButtonPressed();
	void onStopOrderButtonPressed();

	void onAddEmuConsole(HWND, HWND);
	void onAddPvPConsole(HWND, HWND);

	void onOrderFinished(int);
	void onGroupFinished(int, int);
	void onUserFinished(int, int, int);

	//Master Server
	void connected();
	void disconnected();
	void readyRead();

public:
	Ui::ControlPanelClass ui;
	Ui_addOrderDialog addOrderDialog;
	Ui_addOrderDialog addQueueOrderDialog;

	Ui_referralWindow referralsWindow;
	QDialog			  referralsWindowDialog;

	Ui_queueWindow queueWindow;
	QDialog	       queueWindowDialog;

	Ui_debugWindow debugWindow;
	QDialog debugDialog;

	std::map<int, std::vector<RefGroup*>> m_orders;

	QStandardItemModel *orderTableModel;
	QStandardItemModel *queueTableModel;

	QSettings *appSettings;

	int activeOrders;
	bool queueActivated;

	QTcpSocket *masterServerSocket;
	packetHandlerType m_handlerTable[PacketID_MAX];
};

#endif // CONTROLPANEL_H
