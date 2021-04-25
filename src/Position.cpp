#include "Position.h"

Position::Position(int32_t xPosition, int32_t yPosition):
    Position(xPosition, yPosition, 0, 0)
{
    //
}

Position::Position(int32_t xPosition, int32_t yPosition,
                   int32_t xDirection, int32_t yDirection):
    x(xPosition),
    y(yPosition),
    x_d(xDirection),
    y_d(yDirection)
{
    //
}
