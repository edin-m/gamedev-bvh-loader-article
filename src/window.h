#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <string>

#include <unistd.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>


#include <GL/glew.h>
#include <GL/glxew.h>

#include "klib/logger.h"

typedef void (*callback)();
typedef void (*eventCallback)(XEvent&);

class WindowX11
{
    static const std::string tag;

    std::string title;

public:
    WindowX11();
    ~WindowX11();

    void init();
    void main_loop();

    void setDraw(callback f)
    {
        draw = f;
    }

    void setEventHandler(eventCallback f)
    {
        eventHandler = f;
    }

    void redraw()
    {
        draw();
        glXSwapBuffers(display, win);
    }


    callback draw;
    eventCallback eventHandler;

    Display *display;
    Window root;
    XVisualInfo *vi;
    Colormap cmap;
    XSetWindowAttributes swa;
    Window win;
    GLXContext glc;
    XWindowAttributes gwa;
    XEvent xev;

    int width, height;

};

#endif // WINDOW_H
