#-------------------------------------------------
#
# Project created by QtCreator 2017-01-05T16:32:24
#
#-------------------------------------------------

QT  += core gui
QT  += sql
QT  += charts

QMAKE_CXXFLAGS += -fomit-frame-pointer -std=c++11 -pedantic
win32:CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O3

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = QueryCreator
TEMPLATE    = app


SOURCES +=  main.cpp\
            Control/ConnectWindowLogic.cpp \
            Control/MainWindowLogic.cpp \
            SQL/ServerOutput.cpp \
            SQL/SQLEditor.cpp \
            SQL/SQLHighlighter.cpp \
            Tools/DBLogger.cpp \
            Tools/GUITools.cpp \
            View/ConnectWindow.cpp \
            View/MainWindow.cpp

HEADERS  += Control/ConnectWindowLogic.h \
            Control/MainWindowLogic.h \
            SQL/ServerOutput.h \
            SQL/SQLEditor.h \
            SQL/SQLHighlighter.h \
            Tools/Constants.h \
            Tools/DBLogger.h \
            Tools/GUITools.h \
            View/ConnectWindow.h \
            View/MainWindow.h

RESOURCES += resources.qrc

win32:CONFIG(release, debug|release):LIBS += -L$$PWD/../Build/release/ #-lqsqloci
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Build/debug/ -lqsqloci
else:unix: LIBS += -L$$PWD/../Build/ -lqsqloci

INCLUDEPATH += $$PWD/../Build
DEPENDPATH += $$PWD/../Build
