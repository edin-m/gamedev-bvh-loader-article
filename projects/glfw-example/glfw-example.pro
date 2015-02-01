include(../base.pro)

TARGET = glfw-example

unix {

    #GLFW
    GLFW_DIR = $$LIB_DIR/glfw
    INCLUDEPATH += $$GLFW_DIR/include
    LIBS += -L$$GLFW_DIR/src/

    LIBS += -lglfw3
    LIBS += -lXxf86vm -lpthread -lXrandr -lXinerama -lXi -lXcursor
}

SOURCES += main.cpp
