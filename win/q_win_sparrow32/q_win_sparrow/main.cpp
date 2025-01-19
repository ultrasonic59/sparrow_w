#include <QApplication>
#include "q_win_sparrow.h"
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
Q_IMPORT_PLUGIN (QICOPlugin)


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	q_win_sparrow w;
	w.show();

	return a.exec();
}
