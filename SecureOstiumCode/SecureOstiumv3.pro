#-------------------------------------------------
#
# Project created by QtCreator 2016-01-28T17:45:26
#
#-------------------------------------------------

QT       += core gui
QT       += core sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = SecureOstiumv3
target.file = SecureOstiumv3
target.path = /root
INSTALLS += target
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    compare.cpp \
    database.cpp \
    input.cpp \
    pthrd.cpp \
    rfid.cpp \
    status.cpp \
    worker.cpp \
    xbee.cpp \
    button.cpp \
    spi.cpp \
    stm.cpp \
    elock.cpp

HEADERS  += mainwindow.h \
    compare.h \
    database.h \
    input.h \
    pthrd.h \
    rfid.h \
    status.h \
    worker.h \
    xbee.h \
    button.h \
    spi.h \
    stm.h \
    elock.h

FORMS    += mainwindow.ui

LIBS += /home/mike/buildroot/buildroot-2014.11/output/host/usr/lib/gcc/arm-buildroot-linux-gnueabi/4.8.3/libwiringPi.a

LIBS += /home/mike/buildroot/buildroot-2014.11/output/host/usr/lib/gcc/arm-buildroot-linux-gnueabi/4.8.3/libpigpio.a

LIBS += -lpthread

LIBS += -lrt

LIBS += -lasound

RESOURCES = images/rec.qrc
