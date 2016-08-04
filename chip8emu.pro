TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lSDL2

SOURCES += main.cpp \
    chip8cpu.cpp \

HEADERS += \
    chip8cpu.h \
    bitmap_image.hpp

DISTFILES += \
    test.bmp
