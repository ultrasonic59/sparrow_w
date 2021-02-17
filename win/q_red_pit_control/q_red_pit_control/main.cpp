#include <QApplication>
#include "q_red_pit_control.h"
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
Q_IMPORT_PLUGIN (QICOPlugin)


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	q_red_pit_control w;
	w.show();

	return a.exec();
}
