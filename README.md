# Space Invaders
TLDR: Space Invaders clone implemented in C++ & OpenGL.  

This is an ongoing project to learn C++, OpenGL, design patterns, build systems, testing and really anything that comes to mind. There are two branches that are always kept in a state that compiles: main and dev. The sprites used in this project are copied from [Nick Tasios](http://nicktasios.nl/posts/space-invaders-from-scratch-part-1.html) excellent guide to get started with Space Invaders and C++.

## Requirements
* C++17 compliant compiler
* GNU Make
* Cmake version ~3.16 or greater
* OpenGL drivers
* [GLEW](http://glew.sourceforge.net/). Tested with version 2.2.0


## Building and running
### Libraries
Make sure you have version 2.1.0+ of GLEW installed. In addition to GLEW, this project needs to be compiled and linked against [GLFW](https://www.glfw.org/index.html) version 3.3+ and [GLM](https://github.com/g-truc/glm) version 0.9.9+.

### Building
Cmake is used as the build system. There are scripts included in this repository that takes care of everything for you.

GLEW is the only library that is required to be installed on your system. It is recommended that you also install the required versions of GLFW and GLM as shared libraries on your system using your favorite package manager. However if GLFW or GLM is not found then Cmake will attempt to build and link local static versions of these libs.  

There are 2 different build targets configured:
 - Debug - The default target that also configures and builds the tests utilizing the Google Test framework.
 - Release - Only configures and builds an optimized version of the game - nothing else.


```console
foo@bar:space_invaders$ ./configure.sh [release]
foo@bar:space_invaders$ ./build.sh [release]
foo@bar:space_invaders$ ./run.sh [release OR -log]
foo@bar:space_invaders$ ./runtests.sh [ctest]
foo@bar:space_invaders$ ./cleanup [all OR release]
```

All the scripts will use the debug-target by default. With the debug-build you can use the flag `-log` to log to disk. If you wish to build and run a optimized version without the debugging symbols or tests pass the argument `release` without a dash to all the scripts. `runtests.sh` runs all the individual test-binaries. If you have `ctest` installed you can pass ctest as an argument to the script and it will run the tests with ctest and produce a nice overlook of all the tests.  

The scripts will create a build-directory containing the different build environments. A bin-directory containing all the files, possible tests and resources needed to run the game will be created inside the build environments.  

## TODO
* Cleanup and improve the architecture of the codebase
* Add menu system & "normal" features
  * Higscores
  * Levels
  * Pausing and restarting
* Add various features
  * Whatever comes to mind, agile very much
  * Sound effects
  * Improve graphics
  * Improve gameplay
