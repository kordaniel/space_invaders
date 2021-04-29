#ifndef GLOBAL_H
#define GLOBAL_H

/* *************************************************
 * Define DEBUG to compile the project with included
 * error printing and assertions. Define NDEBUG to
 * disable the same for production builds.
 * NOTE: Define only one of these. Also note that
 * your compiler might (un)define these!!!
 */

#define DEBUG
//#define NDEBUG

/* ********************************************** */

#ifdef DEBUG
#include "Timer.h"
#endif

#include <cstdint>
#include <cstddef>
#include <cassert>

enum SpaceobjectType: size_t {
    ALIEN_A    = 0,
    ALIEN_B    = 2,
    ALIEN_C    = 4,
    ALIEN_DEAD = 5,
    PLAYER     = 6,
    BULLET_ALIEN = 7,
    BULLET_PLAYER = 8
};

#endif
