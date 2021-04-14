# Space Invaders
This is a Space invaders clone implemented in c++ & OpenGL. Based on the code from this tutorial: [Space Invaders from Scratch - Part 1 | NICK TASIOS](http://nicktasios.nl/posts/space-invaders-from-scratch-part-1.html). The original code is not very clean in any way, but playable and makes a nice small project to start learning c++.

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
* GNU Make for easy building
* [GLFW](https://www.glfw.org/index.html)
* [GLEW](http://glew.sourceforge.net/)

## Compiling and running
Tested on MacOS 10.15 Catalina. First install the required OpenGL-related libs. For example using HomeBrew:
```console
foo@bar:space_invaders$ brew install glfw glew
```
Compiling using make:
```console
foo@bar:space_invaders$ make
```
This will place the executable inside the bin-directory. Then simply run with:
```console
foo@bar:space_invaders$ ./bin/space_inv
```
Note: The program HAS to be run from the root directory of the repository. When the game is run, it will read the sourcecode for the shaders from the relative directory `res/shaders/Shaders.shader`. 

That's it. Play and enjoy the beauty of what is Space Invaders! Also please report bugs if you find any!

## Notes on Linux
This project has not been tested in linux-based environments, but if you run into troubles compiling try substituting the flags `-lglew -framework OpenGL` with `-lGLEW -lGL`.
