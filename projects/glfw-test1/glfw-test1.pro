#TEMPLATE = app
#CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt

#SOURCES += main.cpp

#include(deployment.pri)
#qtcAddDeployment()

TARGET = kgame
TEMPLATE = app
CONFIG += console
CONFIG -= qt

GIT_ROOT = $$PWD/../..
LIB_DIR  = $$GIT_ROOT/lib

DESTDIR = $$GIT_ROOT/bin

INCLUDEPATH += \
    $$PWD/../include \
    $$PWD/../src

#HEADERS += \
#    $$files($$GIT_ROOT/src/*.h) \
#    $$files($$GIT_ROOT/src/klib/*.h) \
#    $$files($$GIT_ROOT/src/libkbvh2/*.h)

#SOURCES += \
#    $$files($$GIT_ROOT/src/*.cpp) \
#    $$files($$GIT_ROOT/src/klib/*.cpp) \
#    $$files($$GIT_ROOT/src/libkbvh2/*.cpp)

QMAKE_CXXFLAGS += -std=c++11 -std=gnu++11
QMAKE_LFLAGS += -std=c++11 -std=gnu++11

unix {

    #GLEW
    GLEW_DIR = $$LIB_DIR/glew
    INCLUDEPATH += $$GLEW_DIR/include
    LIBS += -L$$GLEW_DIR/lib/ -lGLEW

    #GLM
    INCLUDEPATH += $$LIB_DIR/glm

    LIBS += -lGL -lX11

    #GLFW - static library requires dependencies have put in right order
    GLFW_DIR = $$LIB_DIR/glfw
    INCLUDEPATH += $$GLFW_DIR/include
    LIBS += -L$$GLFW_DIR/src/

    LIBS += -lglfw3
    LIBS += -lXxf86vm -lpthread -lXrandr -lXinerama -lXi -lXcursor
}

win32 {

    # GLEW
    GLEW_DIR = $$LIB_DIR/glew-1.13.0
    INCLUDEPATH += $$GLEW_DIR/include
    LIBS += -L$$GLEW_DIR/bin/Release/Win32

    # GLM
    INCLUDEPATH += $$LIB_DIR/glm

    # GLFW
    GLFW_DIR = $$LIB_DIR/glfw
    INCLUDEPATH += $$GLFW_DIR/include

    release {
        LIBS += -L$$GLFW_DIR/src/Release
    }

    debug {
        LIBS += -L$$GLFW_DIR/src/Debug
    }

    LIBS += -lglfw3
}

# include data files as OTHER_FILES
include($$GIT_ROOT/data/data.pro)


#unix {

#    #GLFW
#    GLFW_DIR = $$LIB_DIR/glfw
#    INCLUDEPATH += $$GLFW_DIR/include
#    LIBS += -L$$GLFW_DIR/src/

#    LIBS += -lglfw3
#    LIBS += -lXxf86vm -lpthread -lXrandr -lXinerama -lXi -lXcursor
#}

#win32 {

#}

SOURCES += main.cpp
