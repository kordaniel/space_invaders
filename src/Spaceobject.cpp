#include "Spaceobject.h"

Spaceobject::Spaceobject(size_t x_pos, size_t y_pos,
                         Sprite &obj_sprite,
                         int _move_speed):
    Size(obj_sprite.width, obj_sprite.height),
    Position(x_pos, y_pos),
    obj_sprite(obj_sprite),
    lives(3),
    move_speed(_move_speed)
{
    //
}

Spaceobject::Spaceobject(size_t x_pos, size_t y_pos, Sprite &obj_sprite):
    Spaceobject(x_pos, y_pos, obj_sprite, 2)
{
    //
}

void Spaceobject::move(void)
{
    switch (x_d) {
        case directions::LEFT:
            //if (x >= move_speed)
            x -= move_speed;
            break;
        case directions::RIGHT:
            //if (x+move_speed < 224)
            x += move_speed;
            break;
        default:
            //
            break;
    }
    switch (y_d) {
        case directions::UP:
            //if (true)
            y += move_speed;
            break;
        case directions::DOWN:
            y -= move_speed;
            break;
        default:
            //
            break;
    }
}

int Spaceobject::getNextX(void)
{
    switch (x_d) {
        case directions::LEFT:
            return x -= move_speed;
            break;
        case directions::RIGHT:
            return x += move_speed;
            break;
        default:
            return x;
            break;
    }
}

int Spaceobject::getNextY(void)
{
    switch (y_d) {
        case directions::UP:
            return y += move_speed;
            break;
        case directions::DOWN:
            return y -= move_speed;
            break;
        default:
            return y;
            break;
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

size_t Spaceobject::GetRightMostX(void)
{
    return x + obj_sprite.width;
}

size_t Spaceobject::GetTopMostY(void)
{
    return y + obj_sprite.height;
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