/********************************************************************************
** Form generated from reading UI file 'queueWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUEUEWINDOW_H
#define UI_QUEUEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_queueWindow
{
public:
	QVBoxLayout *verticalLayout;
	QCheckBox *queueActivatedCheckBox;
	QTableView *queueTable;
	QPushButton *addQueueButton;
	QPushButton *removeQueueButton;
	QLabel *orderLimitLabel;
	QLineEdit *orderLimitEdit;

	void setupUi(QDialog *queueWindow)
	{
		if (queueWindow->objectName().isEmpty())
			queueWindow->setObjectName(QStringLiteral("queueWindow"));
		queueWindow->resize(484, 544);
		verticalLayout = new QVBoxLayout(queueWindow);
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		queueActivatedCheckBox = new QCheckBox(queueWindow);
		queueActivatedCheckBox->setObjectName(QStringLiteral("queueActivatedCheckBox"));

		verticalLayout->addWidget(queueActivatedCheckBox);

		queueTable = new QTableView(queueWindow);
		queueTable->setObjectName(QStringLiteral("queueTable"));
		queueTable->setSelectionBehavior(QAbstractItemView::SelectRows);

		verticalLayout->addWidget(queueTable);

		addQueueButton = new QPushButton(queueWindow);
		addQueueButton->setObjectName(QStringLiteral("addQueueButton"));

		verticalLayout->addWidget(addQueueButton);

		removeQueueButton = new QPushButton(queueWindow);
		removeQueueButton->setObjectName(QStringLiteral("removeQueueButton"));

		verticalLayout->addWidget(removeQueueButton);

		orderLimitLabel = new QLabel(queueWindow);
		orderLimitLabel->setObjectName(QStringLiteral("orderLimitLabel"));

		verticalLayout->addWidget(orderLimitLabel);

		orderLimitEdit = new QLineEdit(queueWindow);
		orderLimitEdit->setObjectName(QStringLiteral("orderLimitEdit"));
		orderLimitEdit->setMaxLength(2);

		verticalLayout->addWidget(orderLimitEdit);

		retranslateUi(queueWindow);

		QMetaObject::connectSlotsByName(queueWindow);
	} // setupUi

	void retranslateUi(QDialog *queueWindow)
	{
		queueWindow->setWindowTitle(QApplication::translate("queueWindow", "Queue List", 0));
		queueActivatedCheckBox->setText(QApplication::translate("queueWindow", "Activated", 0));
		addQueueButton->setText(QApplication::translate("queueWindow", "Add", 0));
		removeQueueButton->setText(QApplication::translate("queueWindow", "Remove", 0));
		orderLimitLabel->setText(QApplication::translate("queueWindow", "Order Limit:", 0));
		orderLimitEdit->setText(QApplication::translate("queueWindow", "0", 0));
	} // retranslateUi
};

namespace Ui {
	class queueWindow : public Ui_queueWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUEUEWINDOW_H
