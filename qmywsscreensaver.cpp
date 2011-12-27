#include "qmywsscreensaver.h"
#include <QDebug>
#include<QScreen>
#include<QDir>
#include<QStringList>
#include<QWidget>

QMyWSScreenSaver::QMyWSScreenSaver():myProcess(NULL)
{
    widget.setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    widget.setAttribute(Qt::WA_PaintOnScreen);
    widget.setAttribute(Qt::WA_TranslucentBackground,TRUE);
    widget.setFixedSize(800,600);
}

QMyWSScreenSaver::~QMyWSScreenSaver(){

}

void QMyWSScreenSaver::restore (){
    if(myProcess){
        myProcess->write("stop\n");
        myProcess->write("quit\n");
        myProcess->kill();
        myProcess->deleteLater();
        myProcess=NULL;
        }
        widget.hide();
}

bool QMyWSScreenSaver::save (int level){
    Q_UNUSED(level);
    QString dirpath = (QCoreApplication::applicationDirPath()+"/media");
    QDir dir(dirpath);
    QStringList filters;
    filters<<"*.avi"<<"*.mp4"<<"*.rmvb"<<"*.rm";
    QStringList namelist = dir.entryList(filters,QDir::Files);
    QString filepath = (dirpath+"/.playlist.txt").trimmed();
    QFile playlistfile(filepath);
    playlistfile.open(QIODevice::ReadWrite);
    QTextStream stream(&playlistfile);
    QStringList temp;
    while(1){
        QString filename = (stream.readLine(75)).trimmed();
        if(filename.isNull())
            break;
        temp<<filename;
    }
    if(temp!=namelist){
        playlistfile.resize(0);
        foreach(QString str ,namelist){
            stream<<str<<endl;
        }
        playlistfile.flush();
    }
    playlistfile.close();
    const QString mplayerPath("/opt/Mplayer/mplayer/bin/mplayer");
    QStringList args;
    args << "-ac"<<"mad"<< "-slave"<<"-quiet"
         <<"-loop"<<"0"<<"-playlist"<<filepath
         <<"-shuffle";
    widget.show();
    myProcess=new QProcess();
    myProcess->start(mplayerPath,args);
    return TRUE;
}

