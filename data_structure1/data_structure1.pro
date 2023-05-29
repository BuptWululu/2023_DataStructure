#-------------------------------------------------
#
# Project created by QtCreator 2023-03-11T16:00:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = data_structure1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        login.cpp \
    registerwidget.cpp \
    mainwindow.cpp \
    navigation.cpp \
    dijsktra.cpp \
    setlabeltext.cpp \
    getpath.cpp \
    journal.cpp \
    txtadd.cpp \
    adddatatime.cpp \
    rootwindow.cpp \
    getweek.cpp \
    addfile.cpp \
    addcurriculum.cpp \
    listwidget.cpp \
    addwidget.cpp \
    alarmwidget.cpp \
    optwidget.cpp \
    txtdelete.cpp \
    alarmnavigation.cpp \
    caution.cpp

HEADERS += \
        login.h \
    registerwidget.h \
    mainwindow.h \
    navigation.h \
    dijsktra.h \
    setlabeltext.h \
    getpath.h \
    journal.h \
    txtadd.h \
    adddatatime.h \
    rootwindow.h \
    getweek.h \
    addfile.h \
    addcurriculum.h \
    listwidget.h \
    addwidget.h \
    alarmwidget.h \
    optwidget.h \
    txtdelete.h \
    alarmnavigation.h \
    caution.h

FORMS += \
        login.ui

RESOURCES += \
    resource.qrc
