#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include <algorithm>

#include "global.h"

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
        Spaceobject(int32_t, int32_t, Sprite&, int32_t);
        Spaceobject(int32_t, int32_t, Sprite &);
    //private:
        Sprite &obj_sprite;
        int32_t lives;
        int32_t move_speed;

        void move(int32_t, int32_t, int32_t, int32_t);
        void SetDirectionUp(bool);
        void SetDirectionDown(bool);
        void SetDirectionLeft(bool);
        void SetDirectionRight(bool);
        void SetDirectionStationary(void);
        int32_t GetRightMostX(void);
        int32_t GetTopMostY(void);
        void ReverseDirection(void);

    private:
        void setHorizontalDirection(directions::Directions, bool);
        void setVerticalDirection(directions::Directions, bool);
};

#endif
