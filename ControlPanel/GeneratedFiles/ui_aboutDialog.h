/********************************************************************************
** Form generated from reading UI file 'aboutDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
	QLabel *pictureLabel;
	QLabel *copyrightLabel;

	void setupUi(QDialog *Dialog)
	{
		if (Dialog->objectName().isEmpty())
			Dialog->setObjectName(QStringLiteral("Dialog"));
		Dialog->resize(400, 300);
		pictureLabel = new QLabel(Dialog);
		pictureLabel->setObjectName(QStringLiteral("pictureLabel"));
		pictureLabel->setGeometry(QRect(25, 22, 351, 211));
		pictureLabel->setLayoutDirection(Qt::LeftToRight);
		pictureLabel->setAlignment(Qt::AlignCenter);
		copyrightLabel = new QLabel(Dialog);
		copyrightLabel->setObjectName(QStringLiteral("copyrightLabel"));
		copyrightLabel->setGeometry(QRect(140, 250, 151, 31));
		QFont font;
		font.setPointSize(14);
		copyrightLabel->setFont(font);

		retranslateUi(Dialog);

		QMetaObject::connectSlotsByName(Dialog);
	} // setupUi

	void retranslateUi(QDialog *Dialog)
	{
		Dialog->setWindowTitle(QApplication::translate("Dialog", "About", 0));
		pictureLabel->setText(QApplication::translate("Dialog", "<html><head/><body><p><img src=\":/ControlPanel/Resources/omdihar.png\"/></p></body></html>", 0));
		copyrightLabel->setText(QApplication::translate("Dialog", "<html><head/><body><p><a href=\"http://www.elitepvpers.com/forum/members/2354584-omdihar.html\"><span style=\" text-decoration: underline; color:#0000ff;\">(c) Omdihar</span></a></p></body></html>", 0));
	} // retranslateUi
};

namespace Ui {
	class Dialog : public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
