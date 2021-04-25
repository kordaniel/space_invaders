#ifndef POSITION_H
#define POSITION_H

#include "global.h"

class Position
{
public:
    Position(int32_t xPosition, int32_t yPosition);
    Position(int32_t xPosition, int32_t yPosition, int32_t xDirection, int32_t yDirection);
    Position(const Position& other) = delete;
    int32_t x;
    int32_t y;
    int32_t x_d;
    int32_t y_d;
};

#endif
