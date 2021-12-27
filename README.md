# Space Invaders
TLDR: Space Invaders clone implemented in C++ & OpenGL.  

This is an ongoing project to learn C++, OpenGL, design patterns, build systems, testing and really anything that comes to mind. There are two branches that are always kept in a state that compiles: main and dev. The sprites used in this project are copied from [Nick Tasios](http://nicktasios.nl/posts/space-invaders-from-scratch-part-1.html) excellent guide to get started with Space Invaders and C++.

## Requirements
* C++17 compliant compiler
* GNU Make
* Cmake version ~3.15 or greater
* [GLFW](https://www.glfw.org/index.html). Tested with version 3.3.3
* [GLEW](http://glew.sourceforge.net/). Tested with version 2.2.0

## Install the required libs
Tested on MacOS 10.15 Catalina using Apple clang version 12. First install the required OpenGL-related libs. For example using HomeBrew:
```console
foo@bar:space_invaders$ brew install glfw glew
```

## Building and running
The project can be built with cmake. The lowest supported version is 3.15 but even 3.10 should work (not tested). Edit row 2 of [CMakeLists.txt](/CMakeLists.txt) if you need to use a lower version. Use the 3 scripts [configure.sh](/configure.sh), [build.sh](/build.sh) and [run.sh](/run.sh) to setup the build environment, build and run the project:
```console
foo@bar:space_invaders$ ./configure.sh [release]
foo@bar:space_invaders$ ./build.sh [release]
foo@bar:space_invaders$ ./run.sh [release OR -log]
```
All the scripts will use the debug-target by default. With the debug-build you can use the flag `-log` to log to disk. If you wish to build and run a optimized version without the debugging symbols pass the argument `release` without a dash to all the scripts.  

The scripts will create a build-directory containing the different build environments. A bin-directory containing all the files and resources needed to run the game will be created inside the build environments.  

## TODO
* Cleanup and improve the architecture of the codebase
* Add tests
* Add menu system & "normal" features
  * Higscores
  * Levels
  * Pausing and restarting
* Add various features
  * Whatever comes to mind, agile very much
  * Sound effects
  * Improve graphics
  * Improve gameplay


## Notes on Linux
This project has **not** been tested in any linux based environment. With that beeing said, it should however work, probably.. Feel free to open a new issue, if you run into any troubles building or running the project!
