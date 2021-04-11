#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include <cstdint>

#include "Size.h"
#include "Position.h"
#include "Sprites.h"

namespace directions
{
    enum Directions: int {
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
        Spaceobject(size_t, size_t, Sprite&, int);
        Spaceobject(size_t, size_t, Sprite &);
    //private:
        Sprite &obj_sprite;
        size_t lives;
        int move_speed;

        void move(void);
        int getNextX(void);
        int getNextY(void);
        void SetDirectionUp(bool);
        void SetDirectionDown(bool);
        void SetDirectionLeft(bool);
        void SetDirectionRight(bool);
        void SetDirectionStationary(void);
        size_t GetRightMostX(void);
        size_t GetTopMostY(void);

    private:
        void setHorizontalDirection(directions::Directions, bool);
        void setVerticalDirection(directions::Directions, bool);
};

#endif
