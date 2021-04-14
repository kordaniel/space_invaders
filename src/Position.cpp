#include "Position.h"

Position::Position(int32_t pos_x, int32_t pos_y):
    Position(pos_x, pos_y, 0, 0)
{
    //
}

Position::Position(int32_t pos_x, int32_t pos_y, int32_t delta_x, int32_t delta_y):
    x(pos_x),
    y(pos_y),
    x_d(delta_x),
    y_d(delta_y)
{
    //
}
