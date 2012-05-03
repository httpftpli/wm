#-------------------------------------------------
#
# Project created by QtCreator 2011-12-23T09:40:51
#
#-------------------------------------------------


QT       += gui
QT       += network

TARGET = windowmanager

TEMPLATE = app

CONFIG += qt debug warn_on

SOURCES += main.cpp \
#    inputpanel.cpp \
 #    qinputmethod.cpp \
 #    qmywsscreensaver.cpp \
    qcomm.cpp \
    msgoutput.cpp \
    qinputmethod.cpp \
    qmywsscreensaver.cpp \
    inputpanel.cpp

FORMS += \
    inputpanel.ui

RESOURCES += \
    resource.qrc

HEADERS += \
#     inputpanel.h \
 #    qinputmethod.h \
 #    qmywsscreensaver.h \
    qcomm.h \
    protocol.h \
    qinputmethod.h \
    qmywsscreensaver.h \
    inputpanel.h

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/windowmanager/bin
    INSTALLS += target
}
