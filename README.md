# Space Invaders
Space invaders clone implemented in c++. Based on the code from this tutorial: [Space Invaders from Scratch - Part 1 | NICK TASIOS](http://nicktasios.nl/posts/space-invaders-from-scratch-part-1.html).

## Requirements
* C++11 compliant environment
* make
* (GLFW)[https://www.glfw.org/index.html]
* (GLEW)[http://glew.sourceforge.net/]

## Compiling and running
Tested to work on MacOS 10.15 Catalina. First install the required OpenGL-related libs. For example using HomeBrew:
```console
foo@bar:space_invaders$ brew install glfw glew
```
Compiling using make:
```console
foo@bar:space_invaders$ make
```
This will place the executable inside the build-directory. Then simply run with:
```console
foo@bar:space_invaders$ ./build/space_inv
```
That's it. Play and enjoy the beauty of what is Space Invaders! Also please report bugs if you find any!

## Notes on Linux
This project has not been tested in linux-based environments, but if you run into troubles compiling try substituting the flags `-lglew -framework OpenGL` with `-lGLEW -lGL`.
