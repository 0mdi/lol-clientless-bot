/********************************************************************************
** Form generated from reading UI file 'controlpanel.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPANEL_H
#define UI_CONTROLPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
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

class Ui_ControlPanelClass
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
	QLabel *cpNameLabel;
	QLineEdit *cpNameLineEdit;
	QMenuBar *menuBar;
	QMenu *HelpMenu;
	QMenu *menuWindow;
	QToolBar *mainToolBar;
	QStatusBar *statusBar;

	void setupUi(QMainWindow *ControlPanelClass)
	{
		if (ControlPanelClass->objectName().isEmpty())
			ControlPanelClass->setObjectName(QStringLiteral("ControlPanelClass"));
		ControlPanelClass->resize(900, 676);
		QIcon icon;
		icon.addFile(QStringLiteral(":/ControlPanel/Resources/quasilogo.ico"), QSize(), QIcon::Normal, QIcon::Off);
		ControlPanelClass->setWindowIcon(icon);
		AboutAction = new QAction(ControlPanelClass);
		AboutAction->setObjectName(QStringLiteral("AboutAction"));
		actionReferals = new QAction(ControlPanelClass);
		actionReferals->setObjectName(QStringLiteral("actionReferals"));
		actionQueue = new QAction(ControlPanelClass);
		actionQueue->setObjectName(QStringLiteral("actionQueue"));
		actionDebug = new QAction(ControlPanelClass);
		actionDebug->setObjectName(QStringLiteral("actionDebug"));
		centralWidget = new QWidget(ControlPanelClass);
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

		cpNameLabel = new QLabel(centralWidget);
		cpNameLabel->setObjectName(QStringLiteral("cpNameLabel"));

		verticalLayout->addWidget(cpNameLabel);

		cpNameLineEdit = new QLineEdit(centralWidget);
		cpNameLineEdit->setObjectName(QStringLiteral("cpNameLineEdit"));

		verticalLayout->addWidget(cpNameLineEdit);

		ControlPanelClass->setCentralWidget(centralWidget);
		menuBar = new QMenuBar(ControlPanelClass);
		menuBar->setObjectName(QStringLiteral("menuBar"));
		menuBar->setGeometry(QRect(0, 0, 900, 21));
		HelpMenu = new QMenu(menuBar);
		HelpMenu->setObjectName(QStringLiteral("HelpMenu"));
		menuWindow = new QMenu(menuBar);
		menuWindow->setObjectName(QStringLiteral("menuWindow"));
		ControlPanelClass->setMenuBar(menuBar);
		mainToolBar = new QToolBar(ControlPanelClass);
		mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
		ControlPanelClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
		statusBar = new QStatusBar(ControlPanelClass);
		statusBar->setObjectName(QStringLiteral("statusBar"));
		ControlPanelClass->setStatusBar(statusBar);

		menuBar->addAction(HelpMenu->menuAction());
		menuBar->addAction(menuWindow->menuAction());
		HelpMenu->addAction(AboutAction);
		menuWindow->addAction(actionReferals);
		menuWindow->addAction(actionQueue);
		menuWindow->addAction(actionDebug);

		retranslateUi(ControlPanelClass);

		QMetaObject::connectSlotsByName(ControlPanelClass);
	} // setupUi

	void retranslateUi(QMainWindow *ControlPanelClass)
	{
		ControlPanelClass->setWindowTitle(QApplication::translate("ControlPanelClass", "ControlPanel", 0));
		AboutAction->setText(QApplication::translate("ControlPanelClass", "About", 0));
		actionReferals->setText(QApplication::translate("ControlPanelClass", "Referrals", 0));
		actionQueue->setText(QApplication::translate("ControlPanelClass", "Queue", 0));
		actionDebug->setText(QApplication::translate("ControlPanelClass", "Debug", 0));
		groupBox->setTitle(QApplication::translate("ControlPanelClass", "Options", 0));
#ifndef QT_NO_TOOLTIP
		startOrderButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
		startOrderButton->setText(QString());
		stopOrderButton->setText(QString());
		removeOrderButton->setText(QString());
		addOrderButton->setText(QString());
		cpNameLabel->setText(QApplication::translate("ControlPanelClass", "Control Panel Name:", 0));
		HelpMenu->setTitle(QApplication::translate("ControlPanelClass", "Help", 0));
		menuWindow->setTitle(QApplication::translate("ControlPanelClass", "Window", 0));
	} // retranslateUi
};

namespace Ui {
	class ControlPanelClass : public Ui_ControlPanelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPANEL_H
