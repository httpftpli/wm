#include "qinputmethod.h"
#include <QApplication>
#include <QScreen>
#include <inputpanel.h>
#include <QDebug>

QInputMethod::QInputMethod() :
    inputwidget(new inputpanel)
{
    connect(inputwidget,SIGNAL(keyGenerated(int,int,Qt::KeyboardModifiers,bool,bool)),
            SLOT(sendContent(int,int,Qt::KeyboardModifiers,bool,bool)));
}


void QInputMethod::updateHandler(int type)
{
    switch(type)
    {
    case QWSInputMethod::FocusIn:
        if(!inputwidget->isLock())
            updatePosition();
        inputwidget->show();
        break;
    case  QWSInputMethod::FocusOut:
        if(!inputwidget->isLock())
            inputwidget->hide();
        break;
    default:
        break;
    }
}

void QInputMethod::updatePosition(){
    /*QWidget *widget = QApplication::focusWidget();
if (!widget)
    return;
QScreen *screen = QScreen::instance();
QRect widgetRect = widget->rect();
QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom() + 2);
panelPos = widget->mapToGlobal(panelPos);
int xdis = panelPos.x()+inputPanel->width()-screen->width();
xdis = qMax(0,xdis);
panelPos.setX(panelPos.x()-xdis);
int ydis = panelPos.y()+inputPanel->height()-screen->height();
if(ydis>0)
    panelPos.setY(panelPos.y()-inputPanel->height()-widgetRect.height()-4);
inputPanel->move(panelPos);*/
}

void  QInputMethod::sendContent(int unicode ,int keycode,
                                Qt::KeyboardModifiers modify,
                                bool ispress,bool autorepeat){
    QWSServer::sendKeyEvent (unicode, keycode, modify,
                             ispress, autorepeat);
}
