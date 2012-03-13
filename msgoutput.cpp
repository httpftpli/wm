#include <stdio.h>
#include <stdlib.h>
#include <QtGlobal>
#include <QTextStream>
#include <QFile>

void MessageOutput(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("WM_Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("WM_Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("WM_Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("WM_Fatal: %1").arg(msg);
    }
    QFile outFile("wm_debuglog.txt");
    if(outFile.size()>(1024*1024*5))
        return;
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

