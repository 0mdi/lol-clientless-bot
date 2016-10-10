/********************************************************************************
** Form generated from reading UI file 'addOrderDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDORDERDIALOG_H
#define UI_ADDORDERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_addOrderDialog
{
public:
	QHBoxLayout *horizontalLayout;
	QGridLayout *gridLayout;
	QLineEdit *refferalCountEditBox;
	QLineEdit *refferalLinkEditBox;
	QLabel *refferalCountLabel;
	QLabel *refferalLinkLabel;
	QPushButton *addOrderButton;
	QGroupBox *groupBox;
	QFormLayout *formLayout;
	QRadioButton *level5RadioButton;
	QRadioButton *level10RadioButton;
	QRadioButton *level30RadioButton;
	QGroupBox *platformGroupBox;
	QWidget *widget;
	QVBoxLayout *verticalLayout;
	QRadioButton *euwRadioButton;
	QRadioButton *euneRadioButton;

	void setupUi(QDialog *addOrderDialog)
	{
		if (addOrderDialog->objectName().isEmpty())
			addOrderDialog->setObjectName(QStringLiteral("addOrderDialog"));
		addOrderDialog->resize(372, 379);
		horizontalLayout = new QHBoxLayout(addOrderDialog);
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
		gridLayout = new QGridLayout();
		gridLayout->setObjectName(QStringLiteral("gridLayout"));
		gridLayout->setHorizontalSpacing(1);
		gridLayout->setContentsMargins(-1, -1, -1, 3);
		refferalCountEditBox = new QLineEdit(addOrderDialog);
		refferalCountEditBox->setObjectName(QStringLiteral("refferalCountEditBox"));

		gridLayout->addWidget(refferalCountEditBox, 4, 1, 1, 1);

		refferalLinkEditBox = new QLineEdit(addOrderDialog);
		refferalLinkEditBox->setObjectName(QStringLiteral("refferalLinkEditBox"));

		gridLayout->addWidget(refferalLinkEditBox, 2, 1, 1, 1);

		refferalCountLabel = new QLabel(addOrderDialog);
		refferalCountLabel->setObjectName(QStringLiteral("refferalCountLabel"));
		QFont font;
		font.setPointSize(10);
		refferalCountLabel->setFont(font);

		gridLayout->addWidget(refferalCountLabel, 3, 0, 1, 2);

		refferalLinkLabel = new QLabel(addOrderDialog);
		refferalLinkLabel->setObjectName(QStringLiteral("refferalLinkLabel"));
		refferalLinkLabel->setFont(font);

		gridLayout->addWidget(refferalLinkLabel, 1, 0, 1, 2);

		addOrderButton = new QPushButton(addOrderDialog);
		addOrderButton->setObjectName(QStringLiteral("addOrderButton"));

		gridLayout->addWidget(addOrderButton, 7, 1, 1, 1);

		groupBox = new QGroupBox(addOrderDialog);
		groupBox->setObjectName(QStringLiteral("groupBox"));
		formLayout = new QFormLayout(groupBox);
		formLayout->setObjectName(QStringLiteral("formLayout"));
		formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
		level5RadioButton = new QRadioButton(groupBox);
		level5RadioButton->setObjectName(QStringLiteral("level5RadioButton"));

		formLayout->setWidget(0, QFormLayout::LabelRole, level5RadioButton);

		level10RadioButton = new QRadioButton(groupBox);
		level10RadioButton->setObjectName(QStringLiteral("level10RadioButton"));

		formLayout->setWidget(1, QFormLayout::LabelRole, level10RadioButton);

		level30RadioButton = new QRadioButton(groupBox);
		level30RadioButton->setObjectName(QStringLiteral("level30RadioButton"));

		formLayout->setWidget(2, QFormLayout::LabelRole, level30RadioButton);

		gridLayout->addWidget(groupBox, 5, 0, 1, 2);

		platformGroupBox = new QGroupBox(addOrderDialog);
		platformGroupBox->setObjectName(QStringLiteral("platformGroupBox"));
		platformGroupBox->setMinimumSize(QSize(100, 100));
		widget = new QWidget(platformGroupBox);
		widget->setObjectName(QStringLiteral("widget"));
		widget->setGeometry(QRect(10, 20, 51, 71));
		verticalLayout = new QVBoxLayout(widget);
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		verticalLayout->setContentsMargins(0, 0, 0, 0);
		euwRadioButton = new QRadioButton(widget);
		euwRadioButton->setObjectName(QStringLiteral("euwRadioButton"));

		verticalLayout->addWidget(euwRadioButton);

		euneRadioButton = new QRadioButton(widget);
		euneRadioButton->setObjectName(QStringLiteral("euneRadioButton"));

		verticalLayout->addWidget(euneRadioButton);

		gridLayout->addWidget(platformGroupBox, 6, 1, 1, 1);

		horizontalLayout->addLayout(gridLayout);

		retranslateUi(addOrderDialog);

		QMetaObject::connectSlotsByName(addOrderDialog);
	} // setupUi

	void retranslateUi(QDialog *addOrderDialog)
	{
		addOrderDialog->setWindowTitle(QApplication::translate("addOrderDialog", "Add Order", 0));
		refferalCountLabel->setText(QApplication::translate("addOrderDialog", "Ref Count:", 0));
		refferalLinkLabel->setText(QApplication::translate("addOrderDialog", "Ref ID:", 0));
		addOrderButton->setText(QApplication::translate("addOrderDialog", "Add Order", 0));
		groupBox->setTitle(QApplication::translate("addOrderDialog", "Type", 0));
		level5RadioButton->setText(QApplication::translate("addOrderDialog", "LEVEL 5", 0));
		level10RadioButton->setText(QApplication::translate("addOrderDialog", "LEVEL 10", 0));
		level30RadioButton->setText(QApplication::translate("addOrderDialog", "LEVEL 30", 0));
		platformGroupBox->setTitle(QApplication::translate("addOrderDialog", "Platform", 0));
		euwRadioButton->setText(QApplication::translate("addOrderDialog", "EUW", 0));
		euneRadioButton->setText(QApplication::translate("addOrderDialog", "EUNE", 0));
	} // retranslateUi
};

namespace Ui {
	class addOrderDialog : public Ui_addOrderDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDORDERDIALOG_H
