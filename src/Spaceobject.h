#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include <cstdint>

#include "Size.h"
#include "Position.h"
#include "Sprites.h"

namespace directions
{
    enum Directions: int32_t {
        STATIONARY =  0,
        UP         =  1,
        DOWN       = -1,
        LEFT       = -2,
        RIGHT      =  2,
    };
} // end namespace directions;


class Spaceobject: public Size, public Position
{
    public:
        Spaceobject(size_t, size_t, Sprite&, directions::Directions, directions::Directions);
        Spaceobject(size_t, size_t, Sprite &);
    //private:
        Sprite &obj_sprite;
        size_t lives;
        directions::Directions move_direction;
        directions::Directions move_speed;
};

#endif
