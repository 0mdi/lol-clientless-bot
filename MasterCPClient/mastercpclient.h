#ifndef MASTERCPCLIENT_H
#define MASTERCPCLIENT_H

#include <QtCore/QtCore>
#include <QtWidgets/QMainWindow>

#include <QMessageBox>
#include <QDialog>

#include <QStandardItem>
#include <QStandardItemModel>

#include <QtNetwork/QTcpSocket>

#include <QSettings>

#include <QList>

#include "ui_mastercpclient.h"
#include "ui_debugWindow.h"
#include "ui_addOrderWindow.h"

#include "protocol.h"

class MasterCPClient;
typedef bool (MasterCPClient::*packetHandlerType)(QByteArray&);

class MasterCPClient : public QMainWindow
{
	Q_OBJECT

public:
	MasterCPClient(QWidget *parent = 0);
	~MasterCPClient();

private:
	void sendPacket(QByteArray packet);

	void registerHandler(packetHandlerType, PacketID packetId);

	bool ordersPacketHandler(QByteArray &packet);

	public slots:
	void connected();
	void disconnected();
	void readyRead();

	void onDebugAction();

	void onAddOrderPressed();
	void onCreateOrderPressed();

private:
	Ui::MasterCpWindow ui;

	//Window stuff
	Ui_debugWindow debugWindow;
	QDialog debugDialog;

	Ui_addOrderWindow addOrderWindow;
	QDialog addOrderDialog;

	QStandardItemModel *orderTableModel;

	QSettings *appSettings;

	QTcpSocket *masterServerSocket;
	packetHandlerType m_handlerTable[PacketID_MAX];
};

#endif // MASTERCPCLIENT_H
