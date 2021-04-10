#include "Spaceobject.h"

Spaceobject::Spaceobject(size_t x_pos, size_t y_pos,
                         Sprite &obj_sprite,
                         directions::Directions move_direction,
                         directions::Directions move_speed):
    Size(obj_sprite.width, obj_sprite.height),
    Position(x_pos, y_pos),
    obj_sprite(obj_sprite),
    lives(3),
    move_direction(move_direction),
    move_speed(move_speed)
{
    //
}

Spaceobject::Spaceobject(size_t x_pos, size_t y_pos, Sprite &obj_sprite):
    Spaceobject(x_pos, y_pos, obj_sprite, directions::STATIONARY,  directions::STATIONARY)
{
    //
}