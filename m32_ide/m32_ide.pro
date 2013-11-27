#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T20:28:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = m32_ide
TEMPLATE = app

SOURCES += \
        main.cpp\
        ide_main_window.cpp \
    ../m32_cpu/m32_cpu.cpp \
    code_editor.cpp

HEADERS += \
        ide_main_window.h \
    ../m32_cpu/m32_cpu.h \
    code_editor.h \
    ../m32_cpu/m32_defines.h

INCLUDEPATH += ../m32_cpu
QMAKE_CXXFLAGS += -std=c++11
