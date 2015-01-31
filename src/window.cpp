#include "window.h"

const std::string WindowX11::tag = "winx11";

WindowX11::WindowX11() : title("Wicked"), width(512), height(512)
{
}

WindowX11::~WindowX11()
{
    GLXContext ctx = glXGetCurrentContext();

    glXMakeCurrent(display, 0, 0);
    glXDestroyContext(display, ctx);

    XDestroyWindow(display, win);
    XFreeColormap(display, cmap);
    XCloseDisplay(display);
}

void WindowX11::init()
{
    display = XOpenDisplay(NULL);
    if( display == NULL )
    {
        klog.e(tag) << "Failed to open display!";

    }
    else
    {
        klog.i(tag) << "Display opened";
    }

    GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

    root = DefaultRootWindow(display);
    vi = glXChooseVisual(display, 0, att);

    if( vi == NULL )
    {
        klog.e(tag) << "Failed to choose visuals!";
    }
    else
    {
        klog.i(tag) << "Visuals chosen";
    }

    cmap = XCreateColormap(display, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.border_pixel = 0;
    swa.background_pixmap = None;
    swa.border_pixmap = None;
    swa.event_mask = ExposureMask | KeyPressMask |
                     KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;

    win = XCreateWindow(display, root, 0, 0, width, height, 0,
                        vi->depth, InputOutput, vi->visual,
                        CWColormap | CWEventMask | CWBackPixmap | CWBorderPixel
                        , &swa);

    XMapWindow(display, win);
    XStoreName(display, win, title.c_str());

    glc = glXCreateContext(display, vi, NULL, GL_TRUE);
    glXMakeCurrent(display, win, glc);

    glEnable(GL_DEPTH_TEST);

    GLenum glew = glewInit();
    if( glew != GLEW_OK )
    {
        klog.e(tag) << "GLEW failed to init!";
    }
    else
    {
        klog.i(tag) << "GLEW initialized";
    }

    const char* extensionName = "GLX_ARB_create_context";
    GLboolean issup = glewIsSupported(extensionName);
    if( issup == GLEW_OK )
    {
        klog.i(tag) << extensionName << "support detected";

        glXMakeCurrent(display, 0, 0);
        glXDestroyContext(display, glc);

        static int visual_attribs[] =
        {
            GLX_X_RENDERABLE,   true,
            GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
            GLX_RENDER_TYPE,    GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
            GLX_DOUBLEBUFFER,   true,
            GLX_RED_SIZE,       8,
            GLX_GREEN_SIZE,     8,
            GLX_BLUE_SIZE,      8,
            GLX_ALPHA_SIZE,     8,
            GLX_DEPTH_SIZE,     24,
            None
        };

        int amp;
        GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display),
                                             visual_attribs, &amp);

        if( !fbc )
        {
            klog.e(tag) << "Failed to choose fb config! Try different.";
        }
        else
        {
            klog.i(tag) << "fb config chosen. You chose wisely.";
        }

        int attrs[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,
            None
        };

        GLXContext newglc = glXCreateContextAttribsARB(display, fbc[0], NULL, true, attrs);
        if( !newglc )
        {
            klog.e(tag) << "Failed to init new glc!";
        }
        else
        {
            klog.i(tag) << "New glc initialized.";
        }

        glXMakeCurrent(display, win, newglc);
        glXSwapBuffers(display, win);


    }
    else
    {
        klog.i(tag) << "No support for extension:" << extensionName;
    }


}

void WindowX11::main_loop()
{
    XEvent event;

    while(true)
    {
        XNextEvent(display, &event);
        switch(event.type)
        {
            case Expose:
                        eventHandler(xev);
                        XGetWindowAttributes(display, win, &gwa);
                        width = gwa.width;
                        height = gwa.height;
                        glViewport(0, 0, width, height);
                        draw();
                        glXSwapBuffers(display, win);
            break;

//            case ConfigureNotify:
//                xce = &(event.xconfigure);
//                width = xce->width;
//                height = xce->height;
////                glViewport(0, 0, width, height);
////                eventHandler(event);
////                draw();
//            break;
        }

        eventHandler(event);

    }

//    while(true)
//    {
//        XNextEvent(display, &xev);

//        if( xev.type == Expose )
//        {
//            eventHandler(xev);
//            XGetWindowAttributes(display, win, &gwa);
//            glViewport(0, 0, gwa.width, gwa.height);
//            draw();
//            glXSwapBuffers(display, win);
//        }
//        else
//        {
//            eventHandler(xev);
//            draw();
//            glXSwapBuffers(display, win);
//        }


//        eventHandler(xev);
//    }
}
