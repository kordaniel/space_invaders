#ifndef POSITION_H
#define POSITION_H

#include "global.h"

class Position
{
public:
    Position(int32_t xPosition, int32_t yPosition);
    Position(int32_t xPosition, int32_t yPosition, int32_t xDirection, int32_t yDirection);
    Position(const Position& other) = delete;
    // TODO: Refactor these member vars to be protected
    int32_t m_x;
    int32_t m_y;
    int32_t m_xD;
    int32_t m_yD;
};

#endif
