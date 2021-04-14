#ifndef POSITION_H
#define POSITION_H

#include "global.h"

class Position
{
    public:
        Position(int32_t, int32_t);
        Position(int32_t, int32_t, int32_t, int32_t);
        int32_t x;
        int32_t y;
        int32_t x_d;
        int32_t y_d;
};

#endif
