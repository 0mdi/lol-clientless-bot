#ifndef ORDERREADER_H
#define ORDERREADER_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QtCore>
#include <QStandardItemModel>
#include <QDragEnterEvent>
#include <QDropEvent>

#include "ui_orderreader.h"

class OrderReader : public QMainWindow
{
	Q_OBJECT

public:
	OrderReader(QWidget *parent = 0);
	~OrderReader();

	void dragEnterEvent(QDragEnterEvent *e);
	void dropEvent(QDropEvent *e);

	void loadOrder(QString fileName);
	void saveOrderDb();

private:
	Ui::OrderReaderClass ui;

	QStandardItemModel *orderTableModel;
};

#endif // ORDERREADER_H
