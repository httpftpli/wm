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

private:
    QWidget widget;
    QProcess *myProcess;
};

#endif // QMYWSSCREENSAVER_H
