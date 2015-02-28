BVH File Loading and Displaying (codebase)
===
http://www.gamedev.net/page/resources/_/technical/game-programming/bvh-file-loading-and-displaying-r3295

Licence
===
1. I do not claim to own anything in data/* folders.
2. Code in test1/** is under GPL2 unless said otherwise in files (only bvh.cpp and bvh.h are exception)

Description
====
Project is located under test1 folder (creative, I know). It is only supported on X11 (which means Linux). 
It would be good to port it with GLFW to multiplatform.

Project features creating OpenGL 3+ context on X11 system, bvh file loading and playing and using 
Vertex Array Objects (VAO) for rendering by GPU. It also features reusable shader class that is put to only basic use here. 
But can be used for various things.

It's main function is to load bvh file and display it and play it. Paths are hardcoded and shortcuts are:
 - Arrows left right up and down move camera
 - b plays backwards
 - c plays forwards
 - v stops playback
 - n frame forward
 - x frame backward

Setup dependencies
===
```
git submodule init lib/glm
git submodule update lib/glm

git submodule init lib/glew
git submodule update lib/glew

git submodule init lib/glfw
git submodule update lib/glfw

cd /.../lib/glew/auto
make

cd /.../lib/glew
make

cd /.../lib/glfw
cmake .
```


Dependencies
===
You will need:
  - linux (check that graphics work with glxgears)
  - GLEW (see below for location)
  - qmake (comes with qt sdk and qt creator)
  - glm (this is included in the repo include/ folder since it is header only library)
  
qmake uses test1.pro to load and link libraries - only dependency being GLEW 
which expects to find it in REPO_ROOT/lib/glew-1.10.0/glew-1.10.0/lib. This can be changed in test1.pro

Of other dependencies you need to have normal build toolchain, libpthread dev, 
and gcc should support flags -std=c++11 -std=gnu++11 which is for any recent gcc no problem.
