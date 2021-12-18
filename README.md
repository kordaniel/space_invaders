# Space Invaders
TLDR: Space Invaders clone implemented in C++ & OpenGL.  

This is an ongoing project to learn C++, OpenGL, design patterns, build systems, testing and really anything that comes to mind. There are two branches that are always kept in a state that compiles: main and dev. The sprites used in this project are copied from [Nick Tasios](http://nicktasios.nl/posts/space-invaders-from-scratch-part-1.html) excellent guide to get started with Space Invaders and C++.

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

## Requirements
* C++17 compliant compiler
* GNU Make & pkg-config for easy building
* [GLFW](https://www.glfw.org/index.html). Tested with version 3.3.3
* [GLEW](http://glew.sourceforge.net/). Tested with version 2.2.0

## Compiling and running
Tested on MacOS 10.15 Catalina using Apple clang version 12. First install the required OpenGL-related libs. For example using HomeBrew:
```console
foo@bar:space_invaders$ brew install glfw glew
```
Build using make:
```console
foo@bar:space_invaders$ make
```
This is equivalent as to building the debug target with `make debug`. It will build and link one executable `bin/space_inv-dbg` with all the debug flags and symbols turned on.
```console
foo@bar:space_invaders$ make release
```
Can be used to build an optimized binary `bin/space_inv` without any debugging symbols.
```console
foo@bar:space_invaders$ make clean
foo@bar:space_invaders$ make cleandebug
foo@bar:space_invaders$ make cleanrelease
```
Can be used to cleanup build- and binary-files.  

To run the various versions simply run the binary:
```console
foo@bar:space_invaders$ ./bin/space_inv
foo@bar:space_invaders$ ./bin/space_inv-dbg
```
Do note that the game will load assets and other resources from the relative directory `./res` from where the game is executed.  

That's it. Play and enjoy the beauty of what is Space Invaders! Also please report bugs if you find any!

## Notes on Linux
This project has **not** been tested in any linux based environment. With that beeing said, it should however work, probably.. Feel free to open a new issue, if you run into any troubles building or running the project!
