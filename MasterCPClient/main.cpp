#include "mastercpclient.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QFile styleSheet(":qdarkstyle/style.qss");
	styleSheet.open(QFile::ReadOnly | QFile::Text);
	QTextStream ts(&styleSheet);
	a.setStyleSheet(ts.readAll());

	MasterCPClient w;
	w.show();
	return a.exec();
}