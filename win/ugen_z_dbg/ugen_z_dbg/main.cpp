#include "ugen_z_dbg.h"
#include <QtWidgets/QApplication>
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
Q_IMPORT_PLUGIN (QICOPlugin)


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ugen_z_dbg w;
	w.show();
	return a.exec();
}
