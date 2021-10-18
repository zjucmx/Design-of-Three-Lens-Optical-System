#-------------------------------------------------
#
# Project created by QtCreator 2020-07-05T22:25:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    part1.c \
    part2.c \
    part3.c \
    aberration_img.c \
    file_read.c

HEADERS  += mainwindow.h \
    C:/Users/HUAWEI/Desktop/optical_cal/opt_main.h \
    C:/Users/HUAWEI/Desktop/optical_cal/part1.h \
    C:/Users/HUAWEI/Desktop/optical_cal/part2.h \
    C:/Users/HUAWEI/Desktop/optical_cal/part3.h \
    opt_main.h \
    part1.h \
    part2.h \
    part3.h \
    aberration_img.h \
    file_read.h \
    svpng.h \
    fonts_table.h

FORMS    += mainwindow.ui
