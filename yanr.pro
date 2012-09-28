#-------------------------------------------------
#
# Project created by QtCreator 2012-09-28T15:01:58
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yanr
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    feedwizard.cpp \
    urlpage.cpp \
    metapage.cpp \
    feed.cpp

HEADERS  += mainwindow.h \
    feedwizard.h \
    urlpage.h \
    metapage.h \
    feed.h

FORMS    += mainwindow.ui \
    feedwizard.ui \
    urlpage.ui \
    metapage.ui
