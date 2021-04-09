#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include "Size.h"
#include "Position.h"
#include "Sprites.h"

class Spaceobject: public Size, public Position
{
    public:
        Spaceobject(size_t, size_t, Sprite&);
    //private:
    Sprite obj_sprite;
    size_t lives;
    int move_direction;
};

#endif
