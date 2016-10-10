/********************************************************************************
** Form generated from reading UI file 'referralWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REFERRALWINDOW_H
#define UI_REFERRALWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_referralWindow
{
public:
	QVBoxLayout *verticalLayout;
	QTabWidget *tabWidget;

	void setupUi(QDialog *referralWindow)
	{
		if (referralWindow->objectName().isEmpty())
			referralWindow->setObjectName(QStringLiteral("referralWindow"));
		referralWindow->resize(530, 455);
		QIcon icon;
		icon.addFile(QStringLiteral("Resources/icon01.ico"), QSize(), QIcon::Normal, QIcon::Off);
		referralWindow->setWindowIcon(icon);
		verticalLayout = new QVBoxLayout(referralWindow);
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		tabWidget = new QTabWidget(referralWindow);
		tabWidget->setObjectName(QStringLiteral("tabWidget"));

		verticalLayout->addWidget(tabWidget);

		retranslateUi(referralWindow);

		tabWidget->setCurrentIndex(-1);

		QMetaObject::connectSlotsByName(referralWindow);
	} // setupUi

	void retranslateUi(QDialog *referralWindow)
	{
		referralWindow->setWindowTitle(QApplication::translate("referralWindow", "Active Referrals", 0));
	} // retranslateUi
};

namespace Ui {
	class referralWindow : public Ui_referralWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REFERRALWINDOW_H
