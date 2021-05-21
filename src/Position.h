#ifndef POSITION_H
#define POSITION_H

#include "global.h"

class Position
{
public:
    Position(int32_t xPosition, int32_t yPosition);
    Position(int32_t xPosition, int32_t yPosition, int32_t xDirection, int32_t yDirection);
    Position(const Position& other) = delete;
    int32_t GetX(void) const;
    int32_t GetY(void) const;
    void SetX(int32_t x);
    void SetY(int32_t y);
    void UpdateX(int32_t xD);
    void UpdateY(int32_t yD);

protected:
    int32_t m_x;
    int32_t m_y;
    int32_t m_xD; // "Direction" or velocity
    int32_t m_yD; // "Direstion" or velocity
};

#endif
