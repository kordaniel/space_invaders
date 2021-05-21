#include "Position.h"

Position::Position(int32_t xPosition, int32_t yPosition):
    Position(xPosition, yPosition, 0, 0)
{
    //
}

Position::Position(int32_t xPosition, int32_t yPosition,
                   int32_t xDirection, int32_t yDirection):
    m_x(xPosition),
    m_y(yPosition),
    m_xD(xDirection),
    m_yD(yDirection)
{
    //
}

int32_t Position::GetX(void) const { return m_x; }
int32_t Position::GetY(void) const { return m_y; }

void Position::SetX(int32_t x) { m_x = x; }
void Position::SetY(int32_t y) { m_y = y; }
void Position::UpdateX(int32_t xD) { m_x += xD; }
void Position::UpdateY(int32_t yD) { m_y += yD; }
