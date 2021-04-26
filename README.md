# Space Invaders
This is a Space invaders clone implemented in C++ & OpenGL. Based on the code from this tutorial: [Space Invaders from Scratch - Part 1 | NICK TASIOS](http://nicktasios.nl/posts/space-invaders-from-scratch-part-1.html). The original code is not very clean in any way, but playable and makes a nice small project to start learning c++.

## TODO
* Refactor to OO paradigma
* Cleanup and restructure codebase
  * Simplify, restructure into manageable independent pieces
  * So that it can be tested
* Add tests
* Keep main- and dev-branches in a state that compiles
* Add various features
  * Whatever comes to mind, agile very much
  * Soundeffects
  * Improve graphics

## Requirements
* C++11 compliant environment
* GNU Make & pkg-config for easy building
* [GLFW](https://www.glfw.org/index.html). Tested with version 3.3.3.
* [GLEW](http://glew.sourceforge.net/). Tested with version 2.2.0.

## Compiling and running
Tested on MacOS 10.15 Catalina using Apple clang version 12. First install the required OpenGL-related libs. For example using HomeBrew:
```console
foo@bar:space_invaders$ brew install glfw glew
```
Build using make:
```console
foo@bar:space_invaders$ make
```
This will place the executable inside the bin-directory. Then simply run with:
```console
foo@bar:space_invaders$ ./bin/space_inv
```

That's it. Play and enjoy the beauty of what is Space Invaders! Also please report bugs if you find any!

## Notes on Linux
This project has **not** been tested in any linux based environment. With that beeing said, it should however work, probably.. Feel free to open a new Issue, if you run into any troubles building or running the project!

