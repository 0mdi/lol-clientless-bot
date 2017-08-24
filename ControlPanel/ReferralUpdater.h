#pragma once
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <Windows.h>
#include "GeneratedFiles/Ui_referralWindow.h"

class ControlPanel;
class ReferralUpdater : public QThread
{
	Q_OBJECT;
public:
	explicit ReferralUpdater(Ui_referralWindow *object, int orderNumber, QObject *parent);
	~ReferralUpdater();

	Ui_referralWindow *m_object;

signals:
	void addEmuConsole(HWND, HWND);
	void addPvPConsole(HWND, HWND);
	void orderFinished(int);
	void groupFinished(int, int);
	void userFinished(int, int, int);

protected:
	void run();

private:
	ControlPanel *m_cpParent;
	int m_orderNumber;
};