/********************************************************************************
** Form generated from reading UI file 'orderreader.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ORDERREADER_H
#define UI_ORDERREADER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OrderReaderClass
{
public:
	QWidget *centralWidget;
	QHBoxLayout *horizontalLayout;
	QTableView *orderTable;
	QMenuBar *menuBar;
	QToolBar *mainToolBar;
	QStatusBar *statusBar;

	void setupUi(QMainWindow *OrderReaderClass)
	{
		if (OrderReaderClass->objectName().isEmpty())
			OrderReaderClass->setObjectName(QStringLiteral("OrderReaderClass"));
		OrderReaderClass->resize(678, 535);
		centralWidget = new QWidget(OrderReaderClass);
		centralWidget->setObjectName(QStringLiteral("centralWidget"));
		horizontalLayout = new QHBoxLayout(centralWidget);
		horizontalLayout->setSpacing(6);
		horizontalLayout->setContentsMargins(11, 11, 11, 11);
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
		orderTable = new QTableView(centralWidget);
		orderTable->setObjectName(QStringLiteral("orderTable"));

		horizontalLayout->addWidget(orderTable);

		OrderReaderClass->setCentralWidget(centralWidget);
		menuBar = new QMenuBar(OrderReaderClass);
		menuBar->setObjectName(QStringLiteral("menuBar"));
		menuBar->setGeometry(QRect(0, 0, 678, 21));
		OrderReaderClass->setMenuBar(menuBar);
		mainToolBar = new QToolBar(OrderReaderClass);
		mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
		OrderReaderClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
		statusBar = new QStatusBar(OrderReaderClass);
		statusBar->setObjectName(QStringLiteral("statusBar"));
		OrderReaderClass->setStatusBar(statusBar);

		retranslateUi(OrderReaderClass);

		QMetaObject::connectSlotsByName(OrderReaderClass);
	} // setupUi

	void retranslateUi(QMainWindow *OrderReaderClass)
	{
		OrderReaderClass->setWindowTitle(QApplication::translate("OrderReaderClass", "OrderReader", 0));
	} // retranslateUi
};

namespace Ui {
	class OrderReaderClass : public Ui_OrderReaderClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ORDERREADER_H
