#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "global.h"
#include "Sprites.h"

#include <memory>
#include <vector>

class Buffer;
class GameObject;
namespace colors
{
    enum Colors: uint32_t;
}

enum class GameObjectType : int32_t {
    // NOTE: This must be kept in synchronization with the mapping
    // defined in GameObject::TypesMap[]
    ALIEN_A       = 0,
    ALIEN_B       = 2,
    ALIEN_C       = 4,
    BULLET_ALIEN  = 6,
    ALIEN_DEAD    = 8,
    PLAYER        = 9,
    BULLET_PLAYER = 10
};

class GraphicsComponent
{
public:
    GraphicsComponent(GameObjectType ownerType);
    ~GraphicsComponent(void);
    void Draw(const GameObject & componentOwner, Buffer & buffer);
    int32_t GetSpriteWidth(void)  const;
    int32_t GetSpriteHeight(void) const;
    void UseSpriteAlive(void);
    void UseSpriteDead(void);

private:
    size_t         m_frame_counter;
    size_t         m_animation_idx;
    size_t         m_animation_freq;
    colors::Colors m_color;

    const std::vector<Sprite *>         * m_sprites_alive;
    const std::vector<Sprite *>         * m_sprites_dead;
    mutable const std::vector<Sprite *> * m_sprites_active;
};

class InputComponent
{
public:
    virtual ~InputComponent(void);
    virtual void Update(GameObject & componentOwner) = 0;
private:
};

class PlayerInputComponent: public InputComponent
{
public:
    virtual void Update(GameObject & componentOwner) override;
private:
};

class GameObject
{
public:
    friend class PlayerInputComponent;
    static GameObjectType constexpr TypesMap[] = {
        // NOTE: Hardcoded mapping from integer => GameObjectType !
        // This must be kept in synchronization with the enum
        GameObjectType::ALIEN_A,      GameObjectType::ALIEN_A,
        GameObjectType::ALIEN_B,      GameObjectType::ALIEN_B,
        GameObjectType::ALIEN_C,      GameObjectType::ALIEN_C,
        GameObjectType::BULLET_ALIEN, GameObjectType::BULLET_ALIEN,
        GameObjectType::ALIEN_DEAD,
        GameObjectType::PLAYER,
        GameObjectType::BULLET_PLAYER
    };
    enum class DirectionHorizontal : int32_t {
        STATIONARY = 0,
        LEFT       = -1,
        RIGHT      = 1
    };
    enum class DirectionVertical : int32_t {
        STATIONARY = 0,
        DOWN       = -1,
        UP         = 1
    };

    static std::unique_ptr<GameObject> CreatePlayer(int32_t lives, int32_t xPos, int32_t yPos, int32_t velocity);
    static std::unique_ptr<GameObject> CreateAlien(GameObjectType type, int32_t lives, int32_t xPos, int32_t yPos, DirectionHorizontal xDir, int32_t velocity);
    static std::unique_ptr<GameObject> CreatePlayerBullet(int32_t lives, int32_t xPos, int32_t yPos, int32_t velocity);
    static std::unique_ptr<GameObject> CreateAlienBullet(int32_t lives, int32_t xPos, int32_t yPos, int32_t velocity);

    GameObject(GameObjectType type, int32_t lives, int32_t deathCounter, int32_t xPos, int32_t yPos, int32_t velocity, DirectionHorizontal xDir, DirectionVertical yDir, InputComponent* inpCmp, bool spawnUnderY = false);
    GameObject(GameObjectType type, int32_t lives, int32_t deathCounter, int32_t xPos, int32_t yPos, int32_t velocity, DirectionHorizontal xDir, DirectionVertical yDir, bool spawnUnderY = false);
    ~GameObject(void);

    void    Draw(Buffer & buffer)                                const;
    int32_t GetWidth(void)                                       const;
    int32_t GetHeight(void)                                      const;
    int32_t GetX(void)                                           const;
    int32_t GetRightMostX(void)                                  const;
    int32_t GetY(void)                                           const;
    int32_t GetTopMostY(void)                                    const;
    int32_t GetMiddleX(void)                                     const;
    int32_t GetLivesAmount(void)                                 const;
    int32_t GetDeathCounter(void)                                const;
    bool    IsAlive(void)                                        const;
    int32_t GetDestroyScore(void)                                const;
    bool    Overlaps(const GameObject & other)                   const;
    bool    VerticalProjectionOverlaps(const GameObject & other) const;
    bool    HandleHit(void);
    bool    Update(const Buffer & buffer);
    void    ReverseHorizontalDirection(void);
    void    MoveDownBySpriteHeight(void);

private:
    void SetDirectionHorizontal(DirectionHorizontal direction);
    void SetDirectionVertical(DirectionVertical direction);
    void move(int32_t xBound, int32_t yBound);

private:
    const GameObjectType      m_type;
    mutable GraphicsComponent m_graphicsComponent;
        // GameObject::Draw is const. GraphicsComponent::Draw mutates it's internal state

    InputComponent *          m_inputComponentPtr;
    int32_t                   m_lives;
    int32_t                   m_deathCounter;
    int32_t                   m_xPos;
    int32_t                   m_yPos;
    int32_t                   m_vel;
    int32_t                   m_xDir;
    int32_t                   m_yDir;
};

GameObject::DirectionHorizontal operator+(const GameObject::DirectionHorizontal & lhs, const GameObject::DirectionHorizontal & rhs);

#endif
