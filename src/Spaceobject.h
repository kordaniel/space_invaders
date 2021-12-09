#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include "global.h"

#include "Size.h"
#include "Position.h"
#include "Io.h"

enum SpaceobjectType {
    ALIEN_A       = 0,
    ALIEN_B       = 2,
    ALIEN_C       = 4,
    BULLET_ALIEN  = 6,
    ALIEN_DEAD    = 8,
    PLAYER        = 9,
    BULLET_PLAYER = 10
};

const SpaceobjectType SpaceobjectTypesMap[] = {
    ALIEN_A, ALIEN_A,
    ALIEN_B, ALIEN_B,
    ALIEN_C, ALIEN_C,
    BULLET_ALIEN, BULLET_ALIEN,
    ALIEN_DEAD,
    PLAYER,
    BULLET_PLAYER
};

namespace directions
{
    enum Directions {
        STATIONARY =  0,
        UP         =  1,
        DOWN       = -1,
        LEFT       = -2,
        RIGHT      =  2,
    };
} // end namespace directions;

class SpaceobjectTypeSpriteSelector
{
public:
    SpaceobjectTypeSpriteSelector(bool animated, size_t maxFrames, size_t min, size_t max, size_t startIdx);
    SpaceobjectTypeSpriteSelector(const SpaceobjectTypeSpriteSelector& other) = delete;
    size_t getCurrentSpriteIdx(void);
    size_t getCurrentSpriteIdx(SpaceobjectType spriteType);

private:
    bool m_animated;
    size_t m_frameChangeFreq;
    size_t m_frameCount;
    size_t m_minIdx;
    size_t m_maxIdx;
    size_t m_currentIdx;
};

class Spaceobject: public Size, public Position
{
public:
    Spaceobject(int32_t xPosition, int32_t yPosition, int32_t xDirection, int32_t yDirection,
                int32_t width, int32_t height,
                int32_t moveSpeed, int32_t lives,
                SpaceobjectType objectType,
                bool animated, size_t animationSpritesNum);
    // Used to create bullet
    Spaceobject(int32_t xPosition, int32_t yPosition, int32_t xDirection, int32_t yDirection,
                int32_t width, int32_t height,
                int32_t moveSpeed, int32_t lives,
                SpaceobjectType objectType);
    Spaceobject(int32_t xPosition, int32_t yPosition,
                int32_t width, int32_t height,
                int32_t moveSpeed,
                SpaceobjectType objectType);
    Spaceobject(const Spaceobject& other) = delete;

    SpaceobjectType m_spriteType;
    SpaceobjectTypeSpriteSelector m_spriteSelector;
    int32_t m_lives;
    int32_t m_move_speed;
    int32_t m_timer = 3;  // TODO: TEMP TEMP, get rid of it. Counts the frames to draw the death sprite

    const SpaceobjectType& getSpriteType(void) const;
    SpaceobjectTypeSpriteSelector& getSpaceObjectTypeSpriteSelector(void);
    bool move(int32_t, int32_t, int32_t, int32_t);
    void SetDirectionUp(bool);
    void SetDirectionDown(bool);
    void SetDirectionLeft(bool);
    void SetDirectionRight(bool);
    void SetDirectionStationary(void);
    int32_t GetRightMostX(void) const;
    int32_t GetTopMostY(void) const;
    int32_t GetMiddleX(void) const;
    void ReverseDirection(void);
    bool overlaps(const Spaceobject& other);
    bool verticalProjectionOverlaps(const Spaceobject& other);
    bool isAlive(void);

private:
    void setHorizontalDirection(directions::Directions, bool);
    void setVerticalDirection(directions::Directions, bool);
};


class Player: public Spaceobject
{
public:
    Player(int32_t xPosition, int32_t yPosition, int32_t width, int32_t height);
    Player(const Player& other) = delete;
    void SetDirectionUp(bool keyPressed);
    void SetDirectionDown(bool keyPressed);
};

class Alien: public Spaceobject
{
public:
    Alien(int32_t xPosition, int32_t yPosition, int32_t xDirection, int32_t yDirection,
          int32_t width, int32_t height,
          int32_t moveSpeed, int32_t lives, SpaceobjectType alienType);

private:
    //
};


#endif
