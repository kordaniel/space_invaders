#include "Spaceobject.h"

Spaceobject::Spaceobject(int32_t x_pos, int32_t y_pos,
                         Sprite &obj_sprite,
                         int32_t _move_speed):
    Size(obj_sprite.width, obj_sprite.height),
    Position(x_pos, y_pos),
    obj_sprite(obj_sprite),
    lives(3),
    move_speed(_move_speed)
{
    //
}

Spaceobject::Spaceobject(int32_t x_pos, int32_t y_pos, Sprite &obj_sprite):
    Spaceobject(x_pos, y_pos, obj_sprite, 2)
{
    //
}

void Spaceobject::move(int32_t lim_west, int32_t lim_east, int32_t lim_north, int32_t lim_south)
{
    if (x_d == directions::LEFT) {
        x = std::max(x - move_speed, 0);
    } else if (x_d == directions::RIGHT) {
        x = std::min(x + move_speed, lim_east - width);
    }

    if (y_d == directions::STATIONARY) {
        return;
    } else if (y_d == directions::DOWN) {
        y = std::max(y - move_speed, lim_south);
    } else if (y_d == directions::UP) {
        y = std::min(y + move_speed, lim_north - height);
    }
}

void Spaceobject::SetDirectionUp(bool keyPressed)
{
    setVerticalDirection(directions::UP, keyPressed);
}

void Spaceobject::SetDirectionDown(bool keyPressed)
{
    setVerticalDirection(directions::DOWN, keyPressed);
}

void Spaceobject::SetDirectionLeft(bool keyPressed)
{
    setHorizontalDirection(directions::LEFT, keyPressed);
}

void Spaceobject::SetDirectionRight(bool keyPressed)
{
    setHorizontalDirection(directions::RIGHT, keyPressed);
}

void Spaceobject::SetDirectionStationary(void)
{
    x_d = directions::STATIONARY;
    y_d = directions::STATIONARY;
}

int32_t Spaceobject::GetRightMostX(void)
{
    return x + obj_sprite.width;
}

int32_t Spaceobject::GetTopMostY(void)
{
    return y + obj_sprite.height;
}

void Spaceobject::ReverseDirection(void)
{
    x_d *= -1;
    y_d *= -1;
}
// Private methods

void Spaceobject::setHorizontalDirection(directions::Directions direction, bool setOrUnset)
{
    x_d += setOrUnset ? direction : -direction;
}

void Spaceobject::setVerticalDirection(directions::Directions direction, bool setOrUnset)
{
    y_d += setOrUnset ? direction : -direction;
}