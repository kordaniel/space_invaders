#ifndef GLOBAL_H
#define GLOBAL_H

#include "Config.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cassert>

#include "Logger.hpp"

#ifndef NDEBUG
    #define assertpair(exp, arg1, arg2)\
        do {\
            if (!(exp)) {\
                Logger::Debug("ASSERTION failed: (%d <=> %s <=> %d):", arg1, #exp, arg2);\
            } else (void)0;\
            assert(exp);\
        } while (false)

    #define NEWoper(variable, command) \
        do {\
            Logger::Debug("%s:%d: Allocating heap mem.", __FILE__, __LINE__);\
            variable = command;\
        } while (false)
#else
    #define assertpair(exp, arg1, arg2) (void)0
    #define NEWoper(variable, command) variable = command
#endif

namespace SI {
    namespace GLOBAL {
        // Global variables
        const double VERSION = std::atoi(SpaceInvaders_VERSION_MAJOR) + 0.1 * std::atoi(SpaceInvaders_VERSION_MINOR);
    } // end namespace GLOBAL

    namespace FILEPATHS {
        const std::string FILE_LOG = "log.txt";

        // ASSETS
        const std::string PATH_SPRITES = "res/spr/";
        const std::string PATH_SHADERS = "res/shaders";
    } // end namespace FILEPATHS
} // end namespace SI

#endif
