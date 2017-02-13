#-------------------------------------------------
#
# Project created by QtCreator 2017-01-05T16:32:24
#
#-------------------------------------------------

QT  += core gui
QT  += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = QueryCreator
TEMPLATE    = app


SOURCES += main.cpp\
        connectwindow.cpp \
        mainwindow.cpp

HEADERS  += Headers/connectwindow.h \
            Headers/mainwindow.h \
            Headers/constants.h

FORMS    += View/connectwindow.ui \
            View/mainwindow.ui
