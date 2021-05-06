#include "Spaceobject.h"

SpaceobjectTypeSpriteSelector::SpaceobjectTypeSpriteSelector(
        bool loop, size_t frameChangeFreq, size_t minIdx, size_t maxIdx, size_t startIdx = 0
)
    : m_animated(loop)
    , m_frameChangeFreq(frameChangeFreq)
    , m_frameCount(0)
    , m_minIdx(minIdx)
    , m_maxIdx(maxIdx)
    , m_currentIdx(startIdx)
{
    //
}

size_t SpaceobjectTypeSpriteSelector::getCurrentSpriteIdx(void)
{
    if (!m_animated) {
        return 0;
    }

    if (m_frameCount > m_frameChangeFreq) {
        m_frameCount = 0;
        ++m_currentIdx;
        if (m_currentIdx > m_maxIdx) {
            m_currentIdx = m_minIdx;
        }
    }

    ++m_frameCount;
    return m_currentIdx;
}

size_t SpaceobjectTypeSpriteSelector::getCurrentSpriteIdx(SpaceobjectType spriteType)
{
    return spriteType + getCurrentSpriteIdx();
}

Spaceobject::Spaceobject(int32_t xPosition, int32_t yPosition, int32_t xDirection, int32_t yDirection,
                         int32_t width, int32_t height,
                         int32_t moveSpeed, int32_t lives,
                         SpaceobjectType objectType,
                         bool animated, size_t animationSpritesNum)
    : Size(width, height)
    , Position(xPosition, yPosition, xDirection, yDirection)
    , m_spriteType(objectType)
    , m_spriteSelector(animated, 15, 0, animationSpritesNum, 0)
    , lives(lives)
    , _move_speed(moveSpeed)
{
    //
}

// Used to create bullet
Spaceobject::Spaceobject(int32_t xPosition, int32_t yPosition, int32_t xDirection, int32_t yDirection,
                         int32_t width, int32_t height,
                         int32_t moveSpeed, int32_t lives,
                         SpaceobjectType objectType)
    : Spaceobject(xPosition, yPosition, xDirection, yDirection, width, height, moveSpeed, lives, objectType, false, 0)
{
    //
}

Spaceobject::Spaceobject(int32_t xPosition, int32_t yPosition,
                         int32_t width, int32_t height,
                         int32_t move_speed,
                         SpaceobjectType objectType):
    Spaceobject(xPosition, yPosition, directions::STATIONARY, directions::STATIONARY, width, height, move_speed, 3, objectType, true, 1)
{
    //
}

const SpaceobjectType& Spaceobject::getSpriteType(void) const
{
    return m_spriteType;
}

SpaceobjectTypeSpriteSelector& Spaceobject::getSpaceObjectTypeSpriteSelector(void)
{
    return m_spriteSelector;
}

///
/// @return true if and only if the horizontal position of this object is equal to either limit.
///
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

int32_t Spaceobject::GetRightMostX(void) const
{
    return x + width;
}

int32_t Spaceobject::GetTopMostY(void) const
{
    return y + height;
}

int32_t Spaceobject::GetMiddleX(void) const
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
bool Spaceobject::overlaps(const Spaceobject& other)
{
    if (GetTopMostY() < other.y + 1 || y + 1 > other.GetTopMostY()) {
        return false;
    }

    return verticalProjectionOverlaps(other);
}

bool Spaceobject::verticalProjectionOverlaps(const Spaceobject& other)
{
    return !(GetRightMostX() < other.x + 1 || x + 1 > other.GetRightMostX());
}

bool Spaceobject::isAlive(void)
{
    return lives > 0;
}


// ============ CLASS: Player  ============
Player::Player(int32_t xPosition, int32_t yPosition, int32_t width, int32_t height):
    Spaceobject(xPosition, yPosition, width, height, 4, SpaceobjectType::PLAYER)
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
             int32_t width, int32_t height,
             int32_t moveSpeed, int32_t lives, SpaceobjectType alienType)
    : Spaceobject(xPosition, yPosition, xDirection, yDirection,
                  width, height, moveSpeed, lives, alienType, true, 1)
{
    //
}
