#include "Spaceobject.h"

Spaceobject::Spaceobject(size_t x_pos, size_t y_pos, Sprite &obj_sprite):
    Size(obj_sprite.width, obj_sprite.height),
    Position(x_pos, y_pos),
    obj_sprite(obj_sprite),
    lives(3),
    move_direction(0)
{
    //
}
