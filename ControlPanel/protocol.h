#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QDataStream>
#include <QDateTime>

#include <exception>

#include "NBinaryStream.hpp"

#define MAGIC 0x1997

enum PacketID : qint8
{
	//ControlPanel Stuff
	ClientOptions, //SClientOptions, RClientOptions
	OrdersPacket, //SOrdersPacket, ROrdersPacket

	//MasterCP Stuff
	AddOrder, //SAddOrder, RAddOrder
	GetOrders, //SGetOrders, RGetOrders

	PacketID_MAX
};

struct PacketHeader
{
	qint16 magic;
	qint16 bodySize;
	PacketID packetId;
};

//R => RECV
//S => SEND

//==>

class RClientOptions
{
public:
	RClientOptions(QByteArray &packet)
	{
		memset(m_version, 0, 32);
		memset(m_cpName, 0, 50);

		Narea::NBaseBinaryStream::Buffer packetBuffer(packet.begin(), packet.end());
		Narea::NBinaryStream binaryStream(&packetBuffer);

		binaryStream.rawRead(0, 32, (byte*)m_version);

		binaryStream.rawRead(32, 50, (byte*)m_cpName);
		binaryStream.rpos(82);

		binaryStream >> m_maxRamAvailable;
		binaryStream >> m_queueActivated;
		binaryStream >> m_orderLimit;
	}

	QString getVersion()
	{
		return m_version;
	}

	QString getCpName()
	{
		return m_cpName;
	}

	qint64 getMaxRam()
	{
		return m_maxRamAvailable;
	}

	bool isQueueActivated()
	{
		return m_queueActivated;
	}

	qint8 getOrderLimit()
	{
		return m_orderLimit;
	}

private:
	char m_version[32];
	char m_cpName[50];
	qint64 m_maxRamAvailable;
	bool m_queueActivated;
	qint8 m_orderLimit;
};

class SClientOptions
{
public:
	SClientOptions()
	{
		memset(m_version, 0, 32);
		memset(m_cpName, 0, 50);
	}

	SClientOptions(QString version, QString cpName, qint64 maxRamAvailable, bool queueActivated, qint8 orderLimit)
		: m_maxRamAvailable(maxRamAvailable)
		, m_queueActivated(queueActivated)
		, m_orderLimit(orderLimit)
	{
		memset(m_version, 0, 32);
		memset(m_cpName, 0, 50);

		strcpy(m_version, version.toStdString().c_str());
		strcpy(m_cpName, cpName.toStdString().c_str());
	}

	QByteArray parse()
	{
		Narea::NBaseBinaryStream::Buffer payloadBuffer;
		Narea::NBaseBinaryStream::Buffer fullPacketBuffer;

		Narea::NBinaryStream binaryStream(&payloadBuffer);
		Narea::NBinaryStream finalStream(&fullPacketBuffer);

		binaryStream.rawWrite(binaryStream.wpos(), 32, (byte*)m_version);
		binaryStream.rawWrite(binaryStream.wpos(), 50, (byte*)m_cpName);
		binaryStream << m_maxRamAvailable;
		binaryStream << m_queueActivated;
		binaryStream << m_orderLimit;

		//Build header
		PacketHeader header;
		header.magic = MAGIC;
		header.bodySize = payloadBuffer.size();
		header.packetId = ClientOptions;

		finalStream << header;
		finalStream << payloadBuffer;

		return QByteArray((char*)fullPacketBuffer.data(), fullPacketBuffer.size());
	}

	void setVersion(QString version)
	{
		strcpy(m_version, version.toStdString().c_str());
	}

	void setCpName(QString cpName)
	{
		strcpy(m_cpName, cpName.toStdString().c_str());
	}

	void setMaxRam(qint64 maxRam)
	{
		m_maxRamAvailable = maxRam;
	}

	void setQueueActivated(bool queueActivated)
	{
		m_queueActivated = queueActivated;
	}

	void setOrderLimit(qint8 orderLimit)
	{
		m_orderLimit = orderLimit;
	}

private:
	char m_version[32];
	char m_cpName[50];
	qint64 m_maxRamAvailable;
	bool m_queueActivated;
	qint8 m_orderLimit;
};

//==>

enum OrderType : qint8
{
	All,
	Paused,
	Started,
	Finished,
	Queue,
	NormalOrder
};

struct Order
{
	Order()
	{
		memset(refID, 0, 30);
		memset(platform, 0, 10);
		memset(cpName, 0, 50);
		memset(levelCap, 0, 10);
	}

	char refID[30];
	qint16 refCount;
	OrderType  orderType;
	qint8 startedTime[sizeof(QDateTime)];
	qint8 finishedTime[sizeof(QDateTime)];
	qint16 finishedCount;
	char platform[10];
	char levelCap[10];

	char cpName[50];
};

class ROrdersPacket
{
public:
	ROrdersPacket(QByteArray &packet)
	{
		Narea::NBaseBinaryStream::Buffer packetBuffer(packet.begin(), packet.end());
		Narea::NBinaryStream binaryStream(&packetBuffer);

		uint64_t dummy;

		binaryStream >> dummy;
		binaryStream >> dummy;
		binaryStream >> m_orderCount;

		//Read orders
		for (int i = 0; i < m_orderCount; ++i)
		{
			Order *newOrder = new Order;
			binaryStream.rawRead(binaryStream.rpos(), sizeof(Order), (byte*)newOrder);
			binaryStream.rpos(binaryStream.rpos() + sizeof(Order));

			m_orders.push_back(newOrder);
		}
	}

	~ROrdersPacket()
	{
		//        for (int i = 0; i < m_orders.size(); ++i)
		//            delete m_orders.at(i);
	}

	QList<Order*> getOrders()
	{
		return m_orders;
	}

private:
	qint32 m_orderCount;
	QList<Order*> m_orders;
};

class SOrdersPacket
{
public:
	SOrdersPacket()
	{
	}

	~SOrdersPacket()
	{
		for (int i = 0; i < m_orders.size(); ++i)
			delete m_orders.at(i);
	}

	QByteArray parse()
	{
		Narea::NBaseBinaryStream::Buffer payloadBuffer;
		Narea::NBaseBinaryStream::Buffer fullPacketBuffer;

		Narea::NBinaryStream binaryStream(&payloadBuffer);
		Narea::NBinaryStream finalStream(&fullPacketBuffer);

		binaryStream << (uint64_t)0xFFFFFFFF;
		binaryStream << (uint64_t)0xFFFFFFFF;
		binaryStream << m_orders.size();

		for (int i = 0; i < m_orders.size(); ++i)
			binaryStream.rawWrite(binaryStream.wpos(), sizeof(Order), (byte*)m_orders.at(i));

		PacketHeader header;
		header.magic = MAGIC;
		header.bodySize = payloadBuffer.size();
		header.packetId = OrdersPacket;

		finalStream << header;
		finalStream << payloadBuffer;

		//MessageBoxA(0, binaryStream.hexDump().c_str(), "", 0);

		return QByteArray((char*)fullPacketBuffer.data(), fullPacketBuffer.size());
	}

	void addOrder(Order *order)
	{
		m_orders.push_back(order);
	}

private:
	QList<Order*> m_orders;
};

//==>

class RAddOrder
{
public:
	RAddOrder(QByteArray &packet)
	{
		Narea::NBaseBinaryStream::Buffer packetBuffer(packet.begin(), packet.end());
		Narea::NBinaryStream binaryStream(&packetBuffer);

		binaryStream.rawRead(0, 30, (byte*)m_refID);
		binaryStream.rpos(binaryStream.rpos() + 30);

		binaryStream >> m_refCount;
		binaryStream >> m_orderType;

		binaryStream.rawRead(binaryStream.rpos(), 10, (byte*)m_platform);
		binaryStream.rpos(binaryStream.rpos() + 10);

		binaryStream.rawRead(binaryStream.rpos(), 10, (byte*)m_levelCap);
	}

	QString getRefID()
	{
		return m_refID;
	}

	qint16 getRefCount()
	{
		return m_refCount;
	}

	OrderType getOrderType()
	{
		return m_orderType;
	}

	QString getPlatform()
	{
		return m_platform;
	}

	QString getLevelCap()
	{
		return m_levelCap;
	}

private:
	char m_refID[30];
	qint16 m_refCount;
	OrderType m_orderType;
	char m_platform[10];
	char m_levelCap[10];
};

class SAddOrder
{
public:
	SAddOrder(QString refID, qint16 refCount, OrderType orderType, QString platform, QString levelCap)
	{
		if (refID.length() > sizeof(m_refID))
			throw "refID.lengt() > sizeof(m_refID)!";

		if (platform.length() > sizeof(m_platform))
			throw "platform.length() > sizeof(m_platform)!";

		if (levelCap.length() > sizeof(m_levelCap))
			throw "levelCap.length() > sizeof(m_levelCap)!";

		strcpy(m_refID, refID.toStdString().c_str());
		m_refCount = refCount;
		m_orderType = orderType;
		strcpy(m_platform, platform.toStdString().c_str());
		strcpy(m_levelCap, levelCap.toStdString().c_str());
	}

	QByteArray parse()
	{
		Narea::NBaseBinaryStream::Buffer payloadBuffer;
		Narea::NBaseBinaryStream::Buffer fullPacketBuffer;

		Narea::NBinaryStream binaryStream(&payloadBuffer);
		Narea::NBinaryStream finalStream(&fullPacketBuffer);

		binaryStream.rawWrite(binaryStream.wpos(), sizeof(m_refID), (byte*)m_refID);
		binaryStream << m_refCount;
		binaryStream << m_orderType;
		binaryStream.rawWrite(binaryStream.wpos(), sizeof(m_platform), (byte*)m_platform);
		binaryStream.rawWrite(binaryStream.wpos(), sizeof(m_levelCap), (byte*)m_levelCap);

		PacketHeader header;
		header.magic = MAGIC;
		header.bodySize = payloadBuffer.size();
		header.packetId = AddOrder;

		finalStream << header;
		finalStream << payloadBuffer;

		return QByteArray((char*)fullPacketBuffer.data(), fullPacketBuffer.size());
	}

private:
	char m_refID[30];
	qint16 m_refCount;
	OrderType m_orderType;
	char m_platform[10];
	char m_levelCap[10];
};

//==>

class RGetOrders
{
public:
	RGetOrders(QByteArray &packet)
	{
		Narea::NBaseBinaryStream::Buffer packetBuffer(packet.begin(), packet.end());
		Narea::NBinaryStream binaryStream(&packetBuffer);

		binaryStream >> m_orderFilter;
	}

	OrderType getOrderFilter()
	{
		return m_orderFilter;
	}

private:
	OrderType m_orderFilter;
};

class SGetOrders
{
public:
	SGetOrders(OrderType orderFilter)
		: m_orderFilter(orderFilter)
	{
	}

	QByteArray parse()
	{
		Narea::NBaseBinaryStream::Buffer payloadBuffer;
		Narea::NBaseBinaryStream::Buffer fullPacketBuffer;

		Narea::NBinaryStream binaryStream(&payloadBuffer);
		Narea::NBinaryStream finalStream(&fullPacketBuffer);

		binaryStream << m_orderFilter;

		PacketHeader header;
		header.magic = MAGIC;
		header.bodySize = payloadBuffer.size();
		header.packetId = GetOrders;

		finalStream << header;
		finalStream << payloadBuffer;

		return QByteArray((char*)fullPacketBuffer.data(), fullPacketBuffer.size());
	}

private:
	OrderType m_orderFilter;
};

//==>

#endif // PROTOCOL_H
