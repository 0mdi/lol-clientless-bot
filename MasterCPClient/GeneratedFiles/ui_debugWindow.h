/********************************************************************************
** Form generated from reading UI file 'debugWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEBUGWINDOW_H
#define UI_DEBUGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_debugWindow
{
public:
	QHBoxLayout *horizontalLayout;
	QTextEdit *debugTextEdit;

	void setupUi(QDialog *debugWindow)
	{
		if (debugWindow->objectName().isEmpty())
			debugWindow->setObjectName(QStringLiteral("debugWindow"));
		debugWindow->resize(521, 459);
		horizontalLayout = new QHBoxLayout(debugWindow);
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
		debugTextEdit = new QTextEdit(debugWindow);
		debugTextEdit->setObjectName(QStringLiteral("debugTextEdit"));
		debugTextEdit->setReadOnly(true);
		debugTextEdit->setOverwriteMode(false);

		horizontalLayout->addWidget(debugTextEdit);

		retranslateUi(debugWindow);

		QMetaObject::connectSlotsByName(debugWindow);
	} // setupUi

	void retranslateUi(QDialog *debugWindow)
	{
		debugWindow->setWindowTitle(QApplication::translate("debugWindow", "Debug", 0));
	} // retranslateUi
};

namespace Ui {
	class debugWindow : public Ui_debugWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUGWINDOW_H
