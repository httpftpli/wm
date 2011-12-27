#ifndef QINPUTMETHOD_H
#define QINPUTMETHOD_H

class inputpanel;
#include <QWSInputMethod>

class QInputMethod : public QWSInputMethod
{
    Q_OBJECT
public:
    explicit QInputMethod();

    void setMicroFocus(int, int);
    void updateHandler(int);

private:
    inputpanel * inputwidget;
    //QString content;
    void updatePosition();

private slots:
     void sendContent(int unicode ,int keycode,
                      Qt::KeyboardModifiers modify,
                      bool ispress,bool autorepeat);
    //void confirmContent();


};

#endif // QINPUTMETHOD_H

