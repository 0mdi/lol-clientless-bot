#include "orderreader.h"

OrderReader::OrderReader(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setAcceptDrops(true);

	orderTableModel = new QStandardItemModel();
	ui.orderTable->setModel(orderTableModel);
}

OrderReader::~OrderReader()
{
	saveOrderDb();
}

void OrderReader::dragEnterEvent(QDragEnterEvent *e)
{
	e->acceptProposedAction();
}

void OrderReader::dropEvent(QDropEvent *e)
{
	auto urlList = e->mimeData()->urls();
	orderTableModel->clear();
	loadOrder(urlList[0].toLocalFile());
}

void OrderReader::loadOrder(QString fileName)
{
	//Load db file
	QFile db(fileName);

	if (db.open(QIODevice::ReadOnly))
	{
		QDataStream stream(&db);

		qint32 rowCount, columnCount;
		stream >> rowCount >> columnCount;

		orderTableModel->setRowCount(rowCount);
		orderTableModel->setColumnCount(columnCount);

		for (int i = 0; i < rowCount; ++i)
		{
			for (int j = 0; j < columnCount; ++j)
			{
				QStandardItem *item = new QStandardItem();
				item->read(stream);
				item->setBackground(QColor(255, 255, 255));
				orderTableModel->setItem(i, j, item);
			}
		}
	}

	db.close();
}

void OrderReader::saveOrderDb()
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