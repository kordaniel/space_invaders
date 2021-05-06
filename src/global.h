#ifndef GLOBAL_H
#define GLOBAL_H

/* **************************************************
 * TODO: !!! Refactor all code to use only NDEBUG !!!
 *
 * Define DEBUG to compile the project with included
 * error printing and assertions. Define NDEBUG to
 * disable the same for production builds.
 * NOTE: Define only one of these. Also note that
 * your compiler might (un)define these!!!
 */

#define DEBUG
//#define NDEBUG
/* *********************************************** */

#ifdef DEBUG
#include "Timer.h"
#endif

#include <cstdint>
#include <cstddef>
#include <string>

#include <cassert>

#ifndef NDEBUG
// NOTE: We havent included "Io.h" here, so if the code wont compile this must be handled.
//       This is because then we would have different includes for DEBUG/RELEASE modes.
    #define assertpair(exp, arg1, arg2)\
        do {\
            if (!(exp))\
                io::print_to_stderr_varargs("ASSERTION failed: (", arg1, " <=> ", #exp, " <=> ", arg2, "):");\
            else (void)0;\
            assert(exp);\
        } while (false)

    #define NEWoper(variable, command) \
        do {\
            io::print_to_stdout_varargs(__FILE__, ", ", __LINE__, ": Allocating heap mem.");\
            variable = command;\
        } while (false)
#else
    #define assertpair(exp, arg1, arg2) (void)0
    #define NEWoper(variable, command) variable = command
#endif

// ASSETS
const std::string SPRITES_PATH = "res/spr/";
const std::string SHADERS_PATH = "res/shaders";

enum SpaceobjectType: size_t {
    ALIEN_A       = 0,
    ALIEN_B       = 2,
    ALIEN_C       = 4,
    ALIEN_DEAD    = 5,
    PLAYER        = 6,
    BULLET_ALIEN  = 7,
    BULLET_PLAYER = 8
};

#endif
