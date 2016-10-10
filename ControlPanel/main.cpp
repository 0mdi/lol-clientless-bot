#include "controlpanel.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//Set damn fusion dark style!
	a.setStyle(QStyleFactory::create("Fusion"));

	//QPalette darkPalette;
	//darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
	//darkPalette.setColor(QPalette::WindowText, Qt::white);
	//darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
	//darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	//darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	//darkPalette.setColor(QPalette::ToolTipText, Qt::white);
	//darkPalette.setColor(QPalette::Text, Qt::red);
	//darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
	//darkPalette.setColor(QPalette::ButtonText, Qt::white);
	//darkPalette.setColor(QPalette::BrightText, Qt::red);
	//darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

	//darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	//darkPalette.setColor(QPalette::HighlightedText, Qt::black);
	//a.setPalette(darkPalette);

	//a.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

	ControlPanel w;
	w.show();
	return a.exec();
}