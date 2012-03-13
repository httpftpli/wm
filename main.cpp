#include <QtGui/QApplication>
#include <QWSServer>
#include <QWSInputMethod>
#include "qinputmethod.h"
#include "qmywsscreensaver.h"
#include "qcomm.h"
#include "QDir"
#include <QtGlobal>

void MessageOutput(QtMsgType type, const char *msg);

int main(int argc, char *argv[])
{
    //qInstallMsgHandler(MessageOutput);
    QApplication a(argc, argv);
    //QDir::setCurrent(QApplication::applicationDirPath());
    //QInputMethod inputmethod;
    QCOMM comm;
    //QWSServer::setCurrentInputMethod(&inputmethod);
    //QMyWSScreenSaver saver;
    //QWSServer::setScreenSaver(&saver);
    //QWSServer::setScreenSaverInterval(60000);
    //QWSServer::setScreenSaverBlockLevel(0);
    return a.exec();
}


