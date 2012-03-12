#-------------------------------------------------
#
# Project created by QtCreator 2011-12-23T09:40:51
#
#-------------------------------------------------


QT       += gui
QT       += network

TARGET = windowmanager

TEMPLATE = app


SOURCES += main.cpp \
#    inputpanel.cpp \
 #    qinputmethod.cpp \
 #    qmywsscreensaver.cpp \
    qcomm.cpp

FORMS += \
    inputpanel.ui

RESOURCES += \
    resource.qrc

HEADERS += \
#     inputpanel.h \
 #    qinputmethod.h \
 #    qmywsscreensaver.h \
    qcomm.h \
    protocol.h
