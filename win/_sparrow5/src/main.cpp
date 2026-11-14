#include "MainWindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QSettings>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("sparrow5");
    QCoreApplication::setApplicationName("sparrow5");
    QSettings current;
    if (current.allKeys().isEmpty()) {
        QSettings previous("emu_thick", "emu_thick");
        const QStringList keys = previous.allKeys();
        for (const QString& key : keys)
            current.setValue(key, previous.value(key));
    }
    QApplication::setStyle("Fusion");
    MainWindow window;
    window.show();
    return app.exec();
}
