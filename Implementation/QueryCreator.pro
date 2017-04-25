#-------------------------------------------------
#
# Project created by QtCreator 2017-01-05T16:32:24
#
#-------------------------------------------------

QT  += core gui
QT  += sql
QT	+= charts

QMAKE_CXXFLAGS += -Wextra -fomit-frame-pointer -std=c++11 -pedantic

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = QueryCreator
TEMPLATE    = app


SOURCES +=  main.cpp\
            connectwindow.cpp \
            mainwindow.cpp \
            dblogger.cpp \
            sqlhighlighter.cpp \
            sqleditor.cpp

HEADERS  += Headers/connectwindow.h \
            Headers/mainwindow.h \
            Headers/constants.h \
            Headers/dblogger.hpp \
            Headers/sqlhighlighter.hpp \
            Headers/sqleditor.h

FORMS    += View/connectwindow.ui \
            View/mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Build/release/ -lqsqloci
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Build/debug/ -lqsqloci
else:unix: LIBS += -L$$PWD/../Build/ -lqsqloci

INCLUDEPATH += $$PWD/../Build
DEPENDPATH += $$PWD/../Build
