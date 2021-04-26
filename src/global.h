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

class SpaceobjectTypeSpriteSelector
{
public:
    SpaceobjectTypeSpriteSelector(bool loop, size_t maxFrames, size_t min, size_t max)
        : m_loop(loop)
        , m_max_frames(maxFrames)
        , m_num_frames(0)
        , m_min(min)
        , m_max(max)
        , m_current(0)
    {
        //
    }

    SpaceobjectTypeSpriteSelector(void)
        : SpaceobjectTypeSpriteSelector(false, 0, 0, 0)
    {
        //
    }

    size_t getCurrentSpriteIdx(SpaceobjectType spriteType)
    {
        if (!m_loop) return spriteType;
        if (m_num_frames > m_max_frames) {
            m_num_frames = 0;
            ++m_current;
            if (m_current > m_max) {
                m_current = m_min;
            }
        }
        m_num_frames += 1;
        return spriteType + m_current;
    }

private:
    bool m_loop = false;
    size_t m_max_frames = 0;
    size_t m_num_frames = 0;
    size_t m_min = 0;
    size_t m_max = 0;
    size_t m_current = 0;
};

#endif
