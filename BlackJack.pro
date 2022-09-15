#-------------------------------------------------
#
# Project created by QtCreator 2022-08-23T16:44:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BlackJack
TEMPLATE = app


SOURCES += main.cpp\
        cardpanel.cpp \
    shuffle.cpp \
    gameplay.cpp \
    cardoutputtext.cpp \
    gamerecord.cpp \
    recorddialog.cpp \
    ruledialog.cpp \
    winratechartdialog.cpp


HEADERS  += cardpanel.h \
    shuffle.h \
    gameplay.h \
    cardoutputtext.h \
    gamerecord.h \
    recorddialog.h \
    ruledialog.h \
    winratechartdialog.h



FORMS    += cardpanel.ui \
    recorddialog.ui \
    ruledialog.ui \
    winratechartdialog.ui

RESOURCES += \
    res.qrc
