#include "Spaceobject.h"

Spaceobject::Spaceobject(int32_t xPosition, int32_t yPosition,
                         int32_t xDirection, int32_t yDirection,
                         int32_t moveSpeed,
                         int32_t lives,
                         Sprite& objectSprite, SpaceobjectType objectType,
                         SpaceobjectTypeSpriteSelector spriteSelector)
    : Size(objectSprite.width, objectSprite.height)
    , Position(xPosition, yPosition, xDirection, yDirection)
    , obj_sprite(objectSprite)
    , m_spriteType(objectType)
    , m_spriteSelector(spriteSelector)
    , lives(lives)
    , _move_speed(moveSpeed)
{
    //
}

Spaceobject::Spaceobject(int32_t xPosition, int32_t yPosition,
                         int32_t xDirection, int32_t yDirection,
                         int32_t moveSpeed,
                         int32_t lives,
                         Sprite& objectSprite, SpaceobjectType objectType)
    : Spaceobject(xPosition, yPosition, xDirection, yDirection, moveSpeed, lives,
                  objectSprite, objectType, SpaceobjectTypeSpriteSelector())
{
    //
}

Spaceobject::Spaceobject(int32_t xPosition, int32_t yPosition,
                         int32_t xDirection, int32_t yDirection,
                         int32_t moveSpeed,
                         int32_t lives,
                         Sprite& objectSprite)
    : Size(objectSprite.width, objectSprite.height)
    , Position(xPosition, yPosition, xDirection, yDirection)
    , obj_sprite(objectSprite)
    , lives(lives)
    , _move_speed(moveSpeed)
{
    // Note: m_spriteType and m_spriteSelector not set!!!!
}

Spaceobject::Spaceobject(int32_t xPosition, int32_t yPosition,
                         int32_t xDirection, int32_t yDirection,
                         int32_t moveSpeed,
                         int32_t lives,
                         Sprite& objectSprite,
                         SpaceobjectTypeSpriteSelector spriteSelector)
        : Size(objectSprite.width, objectSprite.height)
        , Position(xPosition, yPosition, xDirection, yDirection)
        , obj_sprite(objectSprite)
        , m_spriteSelector(spriteSelector)
        , lives(lives)
        , _move_speed(moveSpeed)
{
    // NOTE: m_spriteType not set!!!!!!
}


Spaceobject::Spaceobject(int32_t x_pos, int32_t y_pos,
                         int32_t move_speed,
                         Sprite &obj_sprite, SpaceobjectType objectType):
    Spaceobject(x_pos, y_pos, directions::STATIONARY, directions::STATIONARY, move_speed, 3, obj_sprite, objectType,
                { true, 30, 0, 1 })
{
    //
}

Spaceobject::Spaceobject(int32_t x_pos, int32_t y_pos, Sprite &obj_sprite, SpaceobjectType objectType):
    Spaceobject(x_pos, y_pos, 3, obj_sprite, objectType)
{
    //
}

bool Spaceobject::move(int32_t lim_west, int32_t lim_east, int32_t lim_north, int32_t lim_south)
{
    if (x_d == directions::LEFT) {
        x = std::max(x - _move_speed, 0);
    } else if (x_d == directions::RIGHT) {
        x = std::min(x + _move_speed, lim_east - width);
    }

    if (y_d == directions::STATIONARY) {
        return x == lim_west || GetRightMostX() == lim_east;
    } else if (y_d == directions::DOWN) {
        y = std::max(y - _move_speed, lim_south);
    } else if (y_d == directions::UP) {
        y = std::min(y + _move_speed, lim_north - height);
    }

    return x == lim_west || GetRightMostX() == lim_east;
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
    Spaceobject(xpos, ypos, obj_sprite, SpaceobjectType::PLAYER)
{
    //
}

void Player::SetDirectionUp(bool keyPressed)
{
    assert(keyPressed == keyPressed);
    return; // Player moves only along the horizontal axis
}

void Player::SetDirectionDown(bool keyPressed)
{
    assert(keyPressed == keyPressed);
    return; // Player moves only along the horizontal axis
}

// ============ CLASS: Alien  =============
Alien::Alien(int32_t xPosition, int32_t yPosition, int32_t xDirection, int32_t yDirection,
             int32_t moveSpeed, int32_t lives, size_t alienType, Sprite& objectSprite)
    : Spaceobject(xPosition, yPosition, xDirection, yDirection, moveSpeed, lives, objectSprite, SpaceobjectTypeSpriteSelector(true, 30, 0, 1 ))
{
    switch (alienType) {
        case 1:
            m_spriteType = SpaceobjectType::ALIEN_A;
            break;
        case 2:
            m_spriteType = SpaceobjectType::ALIEN_B;
            break;
        case 3:
            m_spriteType = SpaceobjectType::ALIEN_C;
            break;
        default:
            m_spriteType = SpaceobjectType::ALIEN_DEAD;
            io::print_to_stderr("[ERROR]: Attempting to initialize an unsupported alien type, using ALIEN_A");
            break;
    }
    //m_spriteSelector = { true, 30, 0, 1 };
}
