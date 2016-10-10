/********************************************************************************
** Form generated from reading UI file 'addOrderWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDORDERWINDOW_H
#define UI_ADDORDERWINDOW_H

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

class Ui_addOrderWindow
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
	QWidget *layoutWidget;
	QVBoxLayout *verticalLayout;
	QRadioButton *euwRadioButton;
	QRadioButton *euneRadioButton;

	void setupUi(QDialog *addOrderWindow)
	{
		if (addOrderWindow->objectName().isEmpty())
			addOrderWindow->setObjectName(QStringLiteral("addOrderWindow"));
		addOrderWindow->resize(372, 379);
		horizontalLayout = new QHBoxLayout(addOrderWindow);
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
		gridLayout = new QGridLayout();
		gridLayout->setObjectName(QStringLiteral("gridLayout"));
		gridLayout->setHorizontalSpacing(1);
		gridLayout->setContentsMargins(-1, -1, -1, 3);
		refferalCountEditBox = new QLineEdit(addOrderWindow);
		refferalCountEditBox->setObjectName(QStringLiteral("refferalCountEditBox"));

		gridLayout->addWidget(refferalCountEditBox, 4, 1, 1, 1);

		refferalLinkEditBox = new QLineEdit(addOrderWindow);
		refferalLinkEditBox->setObjectName(QStringLiteral("refferalLinkEditBox"));

		gridLayout->addWidget(refferalLinkEditBox, 2, 1, 1, 1);

		refferalCountLabel = new QLabel(addOrderWindow);
		refferalCountLabel->setObjectName(QStringLiteral("refferalCountLabel"));
		QFont font;
		font.setPointSize(10);
		refferalCountLabel->setFont(font);

		gridLayout->addWidget(refferalCountLabel, 3, 0, 1, 2);

		refferalLinkLabel = new QLabel(addOrderWindow);
		refferalLinkLabel->setObjectName(QStringLiteral("refferalLinkLabel"));
		refferalLinkLabel->setFont(font);

		gridLayout->addWidget(refferalLinkLabel, 1, 0, 1, 2);

		addOrderButton = new QPushButton(addOrderWindow);
		addOrderButton->setObjectName(QStringLiteral("addOrderButton"));

		gridLayout->addWidget(addOrderButton, 7, 1, 1, 1);

		groupBox = new QGroupBox(addOrderWindow);
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

		platformGroupBox = new QGroupBox(addOrderWindow);
		platformGroupBox->setObjectName(QStringLiteral("platformGroupBox"));
		platformGroupBox->setMinimumSize(QSize(100, 100));
		layoutWidget = new QWidget(platformGroupBox);
		layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
		layoutWidget->setGeometry(QRect(10, 20, 51, 71));
		verticalLayout = new QVBoxLayout(layoutWidget);
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		verticalLayout->setContentsMargins(0, 0, 0, 0);
		euwRadioButton = new QRadioButton(layoutWidget);
		euwRadioButton->setObjectName(QStringLiteral("euwRadioButton"));

		verticalLayout->addWidget(euwRadioButton);

		euneRadioButton = new QRadioButton(layoutWidget);
		euneRadioButton->setObjectName(QStringLiteral("euneRadioButton"));

		verticalLayout->addWidget(euneRadioButton);

		gridLayout->addWidget(platformGroupBox, 6, 1, 1, 1);

		horizontalLayout->addLayout(gridLayout);

		retranslateUi(addOrderWindow);

		QMetaObject::connectSlotsByName(addOrderWindow);
	} // setupUi

	void retranslateUi(QDialog *addOrderWindow)
	{
		addOrderWindow->setWindowTitle(QApplication::translate("addOrderWindow", "Add Order", 0));
		refferalCountLabel->setText(QApplication::translate("addOrderWindow", "Ref Count:", 0));
		refferalLinkLabel->setText(QApplication::translate("addOrderWindow", "Ref ID:", 0));
		addOrderButton->setText(QApplication::translate("addOrderWindow", "Add Order", 0));
		groupBox->setTitle(QApplication::translate("addOrderWindow", "Type", 0));
		level5RadioButton->setText(QApplication::translate("addOrderWindow", "LEVEL 5", 0));
		level10RadioButton->setText(QApplication::translate("addOrderWindow", "LEVEL 10", 0));
		level30RadioButton->setText(QApplication::translate("addOrderWindow", "LEVEL 30", 0));
		platformGroupBox->setTitle(QApplication::translate("addOrderWindow", "Platform", 0));
		euwRadioButton->setText(QApplication::translate("addOrderWindow", "EUW", 0));
		euneRadioButton->setText(QApplication::translate("addOrderWindow", "EUNE", 0));
	} // retranslateUi
};

namespace Ui {
	class addOrderWindow : public Ui_addOrderWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDORDERWINDOW_H
