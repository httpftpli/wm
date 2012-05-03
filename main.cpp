#include <QtGui/QApplication>
#include <QWSServer>
#include <QWSInputMethod>
#include "qinputmethod.h"
#include "qmywsscreensaver.h"
#include "qcomm.h"
#include "QDir"
#include <QtGlobal>
#include<QFontDatabase>
#include<QTextCodec>

void MessageOutput(QtMsgType type, const char *msg);

int main(int argc, char *argv[])
{
    //qInstallMsgHandler(MessageOutput);
    QTextCodec *textcode = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(textcode);
    QApplication a(argc, argv);
    QDir::setCurrent(QApplication::applicationDirPath());
    QInputMethod inputmethod;
    QWSServer::setCurrentInputMethod(&inputmethod);
    QMyWSScreenSaver saver;
    QCOMM comm(&saver);
    saver.setPlayerPath(QString("/opt/Mplayer/mplayer/bin/mplayer"));
    QWSServer::setScreenSaver(&saver);
    QWSServer::setScreenSaverInterval(60000);
    QWSServer::setScreenSaverBlockLevel(0);
    return a.exec();
}


