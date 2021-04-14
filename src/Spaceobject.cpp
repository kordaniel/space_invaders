#include "Spaceobject.h"

Spaceobject::Spaceobject(int32_t x_pos, int32_t y_pos,
                         int32_t delta_x, int32_t delta_y,
                         int32_t move_speed,
                         int32_t lives,
                         Sprite &obj_sprite):
    Size(obj_sprite.width, obj_sprite.height),
    Position(x_pos, y_pos, delta_x, delta_y),
    obj_sprite(obj_sprite),
    lives(lives),
    _move_speed(move_speed)
{
    //
}

Spaceobject::Spaceobject(int32_t x_pos, int32_t y_pos,
                         int32_t move_speed,
                         Sprite &obj_sprite):
    Spaceobject(x_pos, y_pos, directions::STATIONARY, directions::STATIONARY, move_speed, 3, obj_sprite)
    //Size(obj_sprite.width, obj_sprite.height),
    //Position(x_pos, y_pos),
    //_move_speed(move_speed),
    //obj_sprite(obj_sprite),
    //lives(3)
{
    //
}

Spaceobject::Spaceobject(int32_t x_pos, int32_t y_pos, Sprite &obj_sprite):
    Spaceobject(x_pos, y_pos, 2, obj_sprite)
{
    //
}

void Spaceobject::move(int32_t lim_west, int32_t lim_east, int32_t lim_north, int32_t lim_south)
{
    if (x_d == directions::LEFT) {
        x = std::max(x - _move_speed, 0);
    } else if (x_d == directions::RIGHT) {
        x = std::min(x + _move_speed, lim_east - width);
    }

    if (y_d == directions::STATIONARY) {
        return;
    } else if (y_d == directions::DOWN) {
        y = std::max(y - _move_speed, lim_south);
    } else if (y_d == directions::UP) {
        y = std::min(y + _move_speed, lim_north - height);
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

int32_t Spaceobject::GetMiddleX(void)
{
    return x + width / 2;
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

///
/// Checks if this object collides with the one given as parameter. 
/// Used for "Colission" detection, see the NOTE. 
///
/// NOTE: This method only checks if the rectangles with the width 
///       and height of the sprites overlap, not if the individual
///       pixels do.
///
/// @param other The object to check if it overlaps with this one.
/// @return true if the two objects overlap, false otherwise.
///
bool Spaceobject::overlaps(Spaceobject& other)
{
    if (GetTopMostY() < other.y + 1 || y + 1 > other.GetTopMostY()) {
        return false;
    }
    if (GetRightMostX() < other.x + 1 || x + 1 > other.GetRightMostX()) {
        return false;
    }

    return true;
}


// ============ CLASS: Player  ============
Player::Player(int32_t xpos, int32_t ypos, Sprite &obj_sprite):
    Spaceobject(xpos, ypos, obj_sprite)
{
    //
}

void Player::SetDirectionUp(bool keyPressed)
{
    return; // Player moves only along the horizontal axis
}

void Player::SetDirectionDown(bool keyPressed)
{
    return; // Player moves only along the horizontal axis
}