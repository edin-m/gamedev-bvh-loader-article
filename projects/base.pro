TARGET = kgame
TEMPLATE = app
CONFIG += console
CONFIG -= qt

GIT_ROOT = $$PWD/..
LIB_DIR  = $$GIT_ROOT/lib

DESTDIR = $$GIT_ROOT/bin

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
    #LIBS += -L$$PWD/../lib/glew-1.10.0/glew-1.10.0/lib/ -lGLEW
    #LIBS += -lGL -lX11 -pthread

    #GLEW
    GLEW_DIR = $$LIB_DIR/glew
    INCLUDEPATH += $$GLEW_DIR/include
    LIBS += -L$$GLEW_DIR/lib/ -lGLEW

    INCLUDEPATH += $$LIB_DIR/glm

    LIBS += -lGL -lX11 -pthread -lpthread
}

# include data files as OTHER_FILES
include($$GIT_ROOT/data/data.pro)
