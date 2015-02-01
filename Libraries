Libraries can be found in lib subdirectory.

GLEW
====

```bash
    $ make -C auto
    $ make
```

If usual suspects don't work try this:
    To compile glew type:
        $ make -C auto
    then
        $ make
    GLEW_DIR/lib should be filled with libraries
    
GLM
===

glm is header only library. No need for building.

GLFW
====

Requires: x11 xrandr xinerama xi xxf86vm xcursor gl
```bash
    $ cmake .
    $ make
```
when linking glfw static library dependecies should be listed after glfw i.e.
```
    LIBS += -lglfw3
    LIBS += -lXxf86vm -lpthread -lXrandr -lXinerama -lXi -lXcursor
```
