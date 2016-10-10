/********************************************************************************
** Form generated from reading UI file 'orderInfoDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ORDERINFODIALOG_H
#define UI_ORDERINFODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_orderInfoDialog
{
public:
	QLabel *refLinkLabel;
	QLineEdit *refLinkLineEdit;
	QLabel *accountsLabel;
	QListWidget *accountsList;

	void setupUi(QDialog *orderInfoDialog)
	{
		if (orderInfoDialog->objectName().isEmpty())
			orderInfoDialog->setObjectName(QStringLiteral("orderInfoDialog"));
		orderInfoDialog->resize(415, 367);
		orderInfoDialog->setMinimumSize(QSize(415, 367));
		orderInfoDialog->setMaximumSize(QSize(415, 367));
		refLinkLabel = new QLabel(orderInfoDialog);
		refLinkLabel->setObjectName(QStringLiteral("refLinkLabel"));
		refLinkLabel->setGeometry(QRect(10, 20, 46, 13));
		refLinkLineEdit = new QLineEdit(orderInfoDialog);
		refLinkLineEdit->setObjectName(QStringLiteral("refLinkLineEdit"));
		refLinkLineEdit->setGeometry(QRect(10, 40, 391, 20));
		accountsLabel = new QLabel(orderInfoDialog);
		accountsLabel->setObjectName(QStringLiteral("accountsLabel"));
		accountsLabel->setGeometry(QRect(10, 70, 61, 16));
		accountsList = new QListWidget(orderInfoDialog);
		accountsList->setObjectName(QStringLiteral("accountsList"));
		accountsList->setGeometry(QRect(10, 90, 391, 271));

		retranslateUi(orderInfoDialog);

		QMetaObject::connectSlotsByName(orderInfoDialog);
	} // setupUi

	void retranslateUi(QDialog *orderInfoDialog)
	{
		orderInfoDialog->setWindowTitle(QApplication::translate("orderInfoDialog", "Information", 0));
		refLinkLabel->setText(QApplication::translate("orderInfoDialog", "Ref ID:", 0));
		accountsLabel->setText(QApplication::translate("orderInfoDialog", "Accounts:", 0));
	} // retranslateUi
};

namespace Ui {
	class orderInfoDialog : public Ui_orderInfoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ORDERINFODIALOG_H
