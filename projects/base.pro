TARGET = kgame
TEMPLATE = app
CONFIG += console
CONFIG -= qt

DESTDIR = ../bin

GIT_ROOT = $$PWD/..

INCLUDEPATH += \
    $$PWD/../include \
    $$PWD/../src

HEADERS += \
    $$files($$GIT_ROOT/src/*.h) \
    $$files($$GIT_ROOT/src/klib/*.h) \
    $$files($$GIT_ROOT/src/libkbvh2/*.h)

SOURCES += \
    $$files($$GIT_ROOT/src/*.cpp) \
    $$files($$GIT_ROOT/src/klib/*.cpp) \
    $$files($$GIT_ROOT/src/libkbvh2/*.cpp)

QMAKE_CXXFLAGS += -std=c++11 -std=gnu++11
QMAKE_LFLAGS += -std=c++11 -std=gnu++11

unix {
    LIBS += -L$$PWD/../lib/glew-1.10.0/glew-1.10.0/lib/ -lGLEW
    LIBS += -lGL -lX11 -pthread
}


# include data files as OTHER_FILES
include($$GIT_ROOT/data/data.pro)
