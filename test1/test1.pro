TARGET = kgame
TEMPLATE = app
CONFIG += console
CONFIG -= qt

DESTDIR = ../bin

SOURCES += \
    src/window.cpp \
    src/shader.cpp \
    main.cpp \
    src/libkbvh2/bvh.cpp

INCLUDEPATH += \
        $$PWD/../include/ \
        $$PWD/src

QMAKE_CXXFLAGS += -std=c++11 -std=gnu++11
QMAKE_LFLAGS += -std=c++11 -std=gnu++11

unix {
    LIBS += -L$$PWD/../lib/glew-1.10.0/glew-1.10.0/lib/ -lGLEW
    LIBS += -lGL -lX11 -pthread
}

HEADERS += \
    src/window.h \
    klib/k_glutils.h \
    klib/logger.h \
    $PWD/../klib/klib.h \
    src/shader.h \
    src/object.h \
    src/camera.h \
    src/libkbvh2/bvh.h \
    src/libkbvh2/util.h \
    src/klib/logger.h \
    src/klib/thread.h

OTHER_FILES += \
    ../data/*.* \
    ../data/shaders/*.* \
    ../data/shaders/freetype/*.* \
    ../data/objects/*.* \
    ../data/images/*.* \
    ../data/shaders/lambert.vert \
    ../data/bvh/*.*
