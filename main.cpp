#include <QtGui/QApplication>
#include <QWSServer>
#include <QWSInputMethod>
#include "qinputmethod.h"
#include "qmywsscreensaver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QInputMethod inputmethod;
    QWSServer::setCurrentInputMethod(&inputmethod);
    QMyWSScreenSaver saver;
    QWSServer::setScreenSaver(&saver);
    QWSServer::setScreenSaverInterval(60000);
    QWSServer::setScreenSaverBlockLevel(0);
    return a.exec();
}
