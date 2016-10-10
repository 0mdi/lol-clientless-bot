/********************************************************************************
** Form generated from reading UI file 'mastercpclient.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MASTERCPCLIENT_H
#define UI_MASTERCPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MasterCpWindow
{
public:
	QAction *AboutAction;
	QAction *actionReferals;
	QAction *actionQueue;
	QAction *actionDebug;
	QWidget *centralWidget;
	QVBoxLayout *verticalLayout;
	QGroupBox *groupBox;
	QGridLayout *gridLayout;
	QPushButton *startOrderButton;
	QPushButton *stopOrderButton;
	QPushButton *removeOrderButton;
	QPushButton *addOrderButton;
	QTableView *orderTable;
	QMenuBar *menuBar;
	QMenu *HelpMenu;
	QMenu *menuWindow;
	QToolBar *mainToolBar;
	QStatusBar *statusBar;

	void setupUi(QMainWindow *MasterCpWindow)
	{
		if (MasterCpWindow->objectName().isEmpty())
			MasterCpWindow->setObjectName(QStringLiteral("MasterCpWindow"));
		MasterCpWindow->resize(900, 676);
		QIcon icon;
		icon.addFile(QStringLiteral(":/ControlPanel/Resources/quasilogo.ico"), QSize(), QIcon::Normal, QIcon::Off);
		MasterCpWindow->setWindowIcon(icon);
		AboutAction = new QAction(MasterCpWindow);
		AboutAction->setObjectName(QStringLiteral("AboutAction"));
		actionReferals = new QAction(MasterCpWindow);
		actionReferals->setObjectName(QStringLiteral("actionReferals"));
		actionQueue = new QAction(MasterCpWindow);
		actionQueue->setObjectName(QStringLiteral("actionQueue"));
		actionDebug = new QAction(MasterCpWindow);
		actionDebug->setObjectName(QStringLiteral("actionDebug"));
		centralWidget = new QWidget(MasterCpWindow);
		centralWidget->setObjectName(QStringLiteral("centralWidget"));
		verticalLayout = new QVBoxLayout(centralWidget);
		verticalLayout->setSpacing(6);
		verticalLayout->setContentsMargins(11, 11, 11, 11);
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		groupBox = new QGroupBox(centralWidget);
		groupBox->setObjectName(QStringLiteral("groupBox"));
		gridLayout = new QGridLayout(groupBox);
		gridLayout->setSpacing(6);
		gridLayout->setContentsMargins(11, 11, 11, 11);
		gridLayout->setObjectName(QStringLiteral("gridLayout"));
		startOrderButton = new QPushButton(groupBox);
		startOrderButton->setObjectName(QStringLiteral("startOrderButton"));
		QIcon icon1;
		icon1.addFile(QStringLiteral(":/ControlPanel/Resources/play.png"), QSize(), QIcon::Normal, QIcon::Off);
		startOrderButton->setIcon(icon1);
		startOrderButton->setIconSize(QSize(32, 32));

		gridLayout->addWidget(startOrderButton, 0, 0, 1, 1);

		stopOrderButton = new QPushButton(groupBox);
		stopOrderButton->setObjectName(QStringLiteral("stopOrderButton"));
		QIcon icon2;
		icon2.addFile(QStringLiteral(":/ControlPanel/Resources/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
		stopOrderButton->setIcon(icon2);
		stopOrderButton->setIconSize(QSize(32, 32));

		gridLayout->addWidget(stopOrderButton, 1, 0, 1, 1);

		removeOrderButton = new QPushButton(groupBox);
		removeOrderButton->setObjectName(QStringLiteral("removeOrderButton"));
		QIcon icon3;
		icon3.addFile(QStringLiteral(":/ControlPanel/Resources/remove.png"), QSize(), QIcon::Normal, QIcon::Off);
		removeOrderButton->setIcon(icon3);
		removeOrderButton->setIconSize(QSize(32, 32));

		gridLayout->addWidget(removeOrderButton, 2, 0, 1, 1);

		addOrderButton = new QPushButton(groupBox);
		addOrderButton->setObjectName(QStringLiteral("addOrderButton"));
		QIcon icon4;
		icon4.addFile(QStringLiteral(":/ControlPanel/Resources/add.png"), QSize(), QIcon::Normal, QIcon::Off);
		addOrderButton->setIcon(icon4);
		addOrderButton->setIconSize(QSize(32, 32));

		gridLayout->addWidget(addOrderButton, 3, 0, 1, 1);

		verticalLayout->addWidget(groupBox);

		orderTable = new QTableView(centralWidget);
		orderTable->setObjectName(QStringLiteral("orderTable"));
		orderTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		orderTable->setAlternatingRowColors(false);
		orderTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
		orderTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		orderTable->setSortingEnabled(false);
		orderTable->horizontalHeader()->setCascadingSectionResizes(false);
		orderTable->horizontalHeader()->setStretchLastSection(true);
		orderTable->verticalHeader()->setCascadingSectionResizes(true);
		orderTable->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
		orderTable->verticalHeader()->setStretchLastSection(false);

		verticalLayout->addWidget(orderTable);

		MasterCpWindow->setCentralWidget(centralWidget);
		menuBar = new QMenuBar(MasterCpWindow);
		menuBar->setObjectName(QStringLiteral("menuBar"));
		menuBar->setGeometry(QRect(0, 0, 900, 21));
		HelpMenu = new QMenu(menuBar);
		HelpMenu->setObjectName(QStringLiteral("HelpMenu"));
		menuWindow = new QMenu(menuBar);
		menuWindow->setObjectName(QStringLiteral("menuWindow"));
		MasterCpWindow->setMenuBar(menuBar);
		mainToolBar = new QToolBar(MasterCpWindow);
		mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
		MasterCpWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
		statusBar = new QStatusBar(MasterCpWindow);
		statusBar->setObjectName(QStringLiteral("statusBar"));
		MasterCpWindow->setStatusBar(statusBar);

		menuBar->addAction(HelpMenu->menuAction());
		menuBar->addAction(menuWindow->menuAction());
		HelpMenu->addAction(AboutAction);
		menuWindow->addAction(actionDebug);

		retranslateUi(MasterCpWindow);

		QMetaObject::connectSlotsByName(MasterCpWindow);
	} // setupUi

	void retranslateUi(QMainWindow *MasterCpWindow)
	{
		MasterCpWindow->setWindowTitle(QApplication::translate("MasterCpWindow", "Master CP", 0));
		AboutAction->setText(QApplication::translate("MasterCpWindow", "About", 0));
		actionReferals->setText(QApplication::translate("MasterCpWindow", "Referrals", 0));
		actionQueue->setText(QApplication::translate("MasterCpWindow", "Queue", 0));
		actionDebug->setText(QApplication::translate("MasterCpWindow", "Debug", 0));
		groupBox->setTitle(QApplication::translate("MasterCpWindow", "Options", 0));
#ifndef QT_NO_TOOLTIP
		startOrderButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
		startOrderButton->setText(QString());
		stopOrderButton->setText(QString());
		removeOrderButton->setText(QString());
		addOrderButton->setText(QString());
		HelpMenu->setTitle(QApplication::translate("MasterCpWindow", "Help", 0));
		menuWindow->setTitle(QApplication::translate("MasterCpWindow", "Window", 0));
	} // retranslateUi
};

namespace Ui {
	class MasterCpWindow : public Ui_MasterCpWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MASTERCPCLIENT_H
