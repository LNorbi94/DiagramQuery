#-------------------------------------------------
#
# Project created by QtCreator 2017-01-05T16:32:24
#
#-------------------------------------------------

QT  += core gui
QT  += sql
QT  += charts

QMAKE_CXXFLAGS += -fomit-frame-pointer -std=c++11 -pedantic

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = QueryCreator
TEMPLATE    = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            sqlhighlighter.cpp \
            sqleditor.cpp \
            Control/ConnectWindowsLogic.cpp \
            Tools/DBLogger.cpp \
            View/ConnectWindow.cpp

HEADERS  += \
            Headers/mainwindow.h \
            Headers/sqlhighlighter.hpp \
            Headers/sqleditor.h \
            Control/ConnectWindowsLogic.h \
            Tools/DBLogger.h \
            Tools/Constants.h \
            View/ConnectWindow.h

FORMS    += View/mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Build/release/
-lqsqloci
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Build/debug/
-lqsqloci
else:unix: LIBS += -L$$PWD/../Build/ -lqsqloci

INCLUDEPATH += $$PWD/../Build
DEPENDPATH += $$PWD/../Build

RESOURCES += \
    resources.qrc
