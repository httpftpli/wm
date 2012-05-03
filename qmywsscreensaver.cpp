#include "qmywsscreensaver.h"
#include <QDebug>
#include<QScreen>
#include<QDir>
#include<QStringList>
#include<QWidget>

QMyWSScreenSaver::QMyWSScreenSaver():myProcess(new QProcess)
{
    widget.setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    widget.setAttribute(Qt::WA_PaintOnScreen);
    widget.setAttribute(Qt::WA_TranslucentBackground,TRUE);
    widget.setFixedSize(800,600);
}

QMyWSScreenSaver::~QMyWSScreenSaver(){
    delete myProcess;
    myProcess = NULL;
}

void QMyWSScreenSaver::restore (){
    stopPlay();
}

bool QMyWSScreenSaver::save (int level){
    Q_UNUSED(level);
    if(playfilename.isEmpty()){
        return play();
    }else{
        bool temp = play(playfilename);
        clearPlayFileName();
        return temp;
    }
    return FALSE;
}

bool QMyWSScreenSaver::play(const QString &filename)
{
    QDir dir("media");
    if(filename.isEmpty()){
        if(dir.exists(QString("default.sh"))){
            widget.show();
            myProcess->start("media/default.sh");
            return TRUE;
        }else
           return playfilelist();
    }else{
        if(!dir.exists(filename))
            return FALSE;
        if(filename.endsWith(QString(".sh"))){
            widget.show();
            myProcess->start("media/"+filename.trimmed());
            return TRUE;
        }else
            return playsinglefile(filename);
    }
    return FALSE;
}

void QMyWSScreenSaver::setPlayerPath(const QString &path)
{
    playerpath = path;
}

bool QMyWSScreenSaver::playfilelist()
{
    QString dirpath("media") ;
    QDir dir(dirpath);
    QStringList filters;
    filters<<"*.avi"<<"*.mp4"<<"*.rmvb"<<"*.rm";
    QStringList namelist = dir.entryList(filters,QDir::Files);
    if(namelist.isEmpty())
        return FALSE;
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
    QStringList args;
    args << "-ac"<<"mad"<< "-slave"<<"-quiet"
         <<"-loop"<<"0"<<"-playlist"<<filepath
        <<"-shuffle";
    widget.show();
    myProcess->start(playerpath,args);
    return TRUE;
}

bool QMyWSScreenSaver::playsinglefile(const QString &filename)
{
    QDir dir("media");
    if(!ismediafileexist(filename))
        return FALSE;
    QStringList args;
    args << "-ac"<<"mad"<< "-slave"<<"-quiet"
            <<dir.absoluteFilePath(filename);
    widget.show();
    myProcess->start(playerpath,args);
    return TRUE;
}

bool QMyWSScreenSaver::ismediafileexist(const QString &filename)
{
    QDir dir("media");
    if(!dir.exists(filename))
        return FALSE;
    if(!filename.endsWith(QString(".avi")))
        return TRUE;
    if(!filename.endsWith(QString(".mp4")))
        return TRUE;
    if(!filename.endsWith(QString(".rmvb")))
        return TRUE;
    if(!filename.endsWith(QString("*.rm")))
        return TRUE;
    return FALSE;
}

void QMyWSScreenSaver::stopPlay()
{
    if(myProcess->state()!=QProcess::NotRunning){
        myProcess->write("stop\n");
        myProcess->write("quit\n");
        if(!myProcess->waitForFinished (1000))
            myProcess->kill();
        widget.hide();
    }
}

void QMyWSScreenSaver::setPlayFileName(const QString &filename)
{
    playfilename = filename;
}

void QMyWSScreenSaver::clearPlayFileName()
{
    playfilename .clear();
}

