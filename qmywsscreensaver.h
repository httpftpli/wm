#ifndef QMYWSSCREENSAVER_H
#define QMYWSSCREENSAVER_H

#include <QWSScreenSaver>
#include<QProcess>
#include<QWidget>

class QMyWSScreenSaver : public QWSScreenSaver
{
public:
    QMyWSScreenSaver();
    virtual	~QMyWSScreenSaver ();
    virtual void restore ();
    virtual bool save (int level);
    bool play(const QString &filename = QString());
    void setPlayerPath(const QString &path);
    void stopPlay();
    void setPlayFileName(const QString &filename);
    void clearPlayFileName();

private:
    QWidget widget;
    QProcess *myProcess;
    QString playerpath;
    QString  playfilename;
    bool playfilelist();
    bool playsinglefile(const QString &filename);
    bool ismediafileexist(const QString &filename);
};

#endif // QMYWSSCREENSAVER_H
