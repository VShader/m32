TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    m32_cpu.cpp

HEADERS += \
    m32_cpu.h \
    m32_defines.h

QMAKE_CXXFLAGS += -std=c++11
