#include "GameObject.h"

#include "Buffer.h" // Includes GLFW
#include "Input.h"

#include <type_traits>


// GRAPHICSCOMPONENT
// -----------------
GraphicsComponent::GraphicsComponent(GameObjectType ownerType)
    : m_frame_counter(0)
    , m_animation_idx(0)
    , m_animation_freq(15)
    , m_color(colors::Colors::ORANGE)
    , m_sprites_alive(Sprites::GetInstance().GetSpriteAlive(ownerType))
    , m_sprites_dead(Sprites::GetInstance().GetSpriteDead(ownerType))
    , m_sprites_active(m_sprites_alive)
{
    //
}

GraphicsComponent::~GraphicsComponent(void)
{
    //
}

void GraphicsComponent::Draw(const GameObject & componentOwner, Buffer & buffer)
{
    const std::vector<Sprite *> & activeSprites = *m_sprites_active;
    if (++m_frame_counter > m_animation_freq) {
        m_frame_counter = 0;
        ++m_animation_idx;
        if (m_animation_idx >= activeSprites.size()) {
            m_animation_idx = 0;
        }
    }
    buffer.drawSprite(componentOwner.GetX(), componentOwner.GetY(), *(activeSprites[m_animation_idx]), m_color);
}

int32_t GraphicsComponent::GetSpriteWidth(void)  const { return (*m_sprites_active)[m_animation_idx]->GetWidth(); }
int32_t GraphicsComponent::GetSpriteHeight(void) const { return (*m_sprites_active)[m_animation_idx]->GetHeight(); }

void GraphicsComponent::UseSpriteAlive(void)
{
    if (m_sprites_alive == nullptr) {
        return;
    }
    m_sprites_active = m_sprites_alive;
    m_frame_counter = 0;
    m_animation_idx = 0;
}

void GraphicsComponent::UseSpriteDead(void)
{
    if (m_sprites_dead == nullptr) {
        return;
    }
    m_sprites_active = m_sprites_dead;
    m_frame_counter = 0;
    m_animation_idx = 0;
}

// INPUTCOMPONENT
InputComponent::~InputComponent(void)
{
    //
}

// PLAYERINPUTCOMPONENT
// --------------------
void PlayerInputComponent::Update(GameObject & componentOwner)
{
    GameObject::DirectionHorizontal newHorDir = GameObject::DirectionHorizontal::STATIONARY;

    if (KeyInput::GetInstance().IsPressed(GLFW_KEY_LEFT)) {
        newHorDir = newHorDir + GameObject::DirectionHorizontal::LEFT;
    }
    if (KeyInput::GetInstance().IsPressed(GLFW_KEY_RIGHT)) {
        newHorDir = newHorDir + GameObject::DirectionHorizontal::RIGHT;
    }

    componentOwner.SetDirectionHorizontal(newHorDir);
}

// GAMEOBJECT
// ----------
std::unique_ptr<GameObject> GameObject::CreatePlayer(int32_t lives, int32_t xPos, int32_t yPos, int32_t velocity)
{
    return std::make_unique<GameObject>(
        GameObjectType::PLAYER, lives, 1, xPos, yPos, velocity,
        DirectionHorizontal::STATIONARY, DirectionVertical::STATIONARY,
        new PlayerInputComponent()
    );
}

std::unique_ptr<GameObject> GameObject::CreateAlien(GameObjectType type, int32_t lives, int32_t xPos, int32_t yPos, DirectionHorizontal xDir, int32_t velocity)
{
#ifndef NDEBUG
    using T = std::underlying_type_t<GameObjectType>;
    assert(0 <= static_cast<T>(type) && static_cast<T>(type) <= 5);
#endif
    return std::make_unique<GameObject>(type, lives, 3, xPos, yPos, velocity, xDir, DirectionVertical::STATIONARY);
}

std::unique_ptr<GameObject> GameObject::CreatePlayerBullet(int32_t lives, int32_t xPos, int32_t yPos, int32_t velocity)
{
    return std::make_unique<GameObject>(
        GameObjectType::BULLET_PLAYER, lives, 1, xPos, yPos, velocity,
        DirectionHorizontal::STATIONARY, DirectionVertical::UP
    );
}

std::unique_ptr<GameObject> GameObject::CreateAlienBullet(int32_t lives, int32_t xPos, int32_t yPos, int32_t velocity)
{
    return std::make_unique<GameObject>(
        GameObjectType::BULLET_ALIEN, lives, 1, xPos, yPos, velocity,
        DirectionHorizontal::STATIONARY, DirectionVertical::DOWN, true
    );
}

GameObject::GameObject(GameObjectType type, int32_t lives, int32_t deathCounter, int32_t xPos, int32_t yPos, int32_t velocity, DirectionHorizontal xDir, DirectionVertical yDir, InputComponent* inputCmpPtr, bool spawnUnderY)
    : m_type(type)
    , m_graphicsComponent(type)
    , m_inputComponentPtr(inputCmpPtr)
    , m_lives(lives)
    , m_deathCounter(deathCounter)
    , m_xPos(xPos - (GetWidth() / 2))
    , m_yPos(spawnUnderY ? yPos - GetHeight() : yPos)
    , m_vel(velocity)
    , m_xDir(static_cast<int32_t>(xDir))
    , m_yDir(static_cast<int32_t>(yDir))
{
    //
}

GameObject::GameObject(GameObjectType type, int32_t lives, int32_t deathCounter, int32_t xPos, int32_t yPos, int32_t velocity, DirectionHorizontal xDir, DirectionVertical yDir, bool spawnUnderY)
    : GameObject(type, lives, deathCounter, xPos, yPos, velocity, xDir, yDir, nullptr, spawnUnderY)
{
    //
}

GameObject::~GameObject(void)
{
    if (m_inputComponentPtr != nullptr) {
        delete m_inputComponentPtr;
    }
}

void    GameObject::Draw(Buffer & buffer) const { m_graphicsComponent.Draw(*this, buffer); }
int32_t GameObject::GetWidth(void)        const { return m_graphicsComponent.GetSpriteWidth(); };
int32_t GameObject::GetHeight(void)       const { return m_graphicsComponent.GetSpriteHeight(); };
int32_t GameObject::GetX(void)            const { return m_xPos; }
int32_t GameObject::GetRightMostX(void)   const { return GetX() + GetWidth(); }
int32_t GameObject::GetY(void)            const { return m_yPos; }
int32_t GameObject::GetTopMostY(void)     const { return GetY() + GetHeight(); }
int32_t GameObject::GetMiddleX(void)      const { return GetX() + (GetWidth() / 2); };
int32_t GameObject::GetLivesAmount(void)  const { return m_lives; }
int32_t GameObject::GetDeathCounter(void) const { return m_deathCounter; }
bool    GameObject::IsAlive(void)         const { return GetLivesAmount() > 0; }

int32_t GameObject::GetDestroyScore(void) const
{
    int32_t score = static_cast<int32_t>(m_type);
    assert(0 <= score && score <= 4); // This is only queried of GameObjects that are alientypes!
    return score / 2;
}

// TODO: Old code -> Read & refactor(?)
bool GameObject::Overlaps(const GameObject & other) const
{ 
    if (GetTopMostY() < other.GetY() + 1 || GetY() + 1 > other.GetTopMostY())
    {
        return false;
    }
    return VerticalProjectionOverlaps(other);
}

// TODO: Old code -> Read & refactor(?)
bool GameObject::VerticalProjectionOverlaps(const GameObject & other) const
{
    return !(GetRightMostX() < other.GetX() + 1 || GetX() + 1 > other.GetRightMostX());
}

bool GameObject::HandleHit(void)
{
    bool isAlive = IsAlive();
    --m_lives;

    if (isAlive == IsAlive()) {
        return isAlive;
    } else if (isAlive) {
        // alive => dead
        m_graphicsComponent.UseSpriteDead();
        return false;
    } else {
        // dead => alive
        m_graphicsComponent.UseSpriteAlive();
        return true;
    }
}

///
/// @return true iff this object is alive and the horizontal position of this object is equal to buffer bounds.
bool GameObject::Update(const Buffer & buffer)
{
    if (!IsAlive()) {
        --m_deathCounter;
        return false;
    }

    if (m_inputComponentPtr != nullptr) {
        m_inputComponentPtr->Update(*this);
    }

    move(buffer.GetWidth(), buffer.GetHeight());
    return GetX() <= 0 || GetRightMostX() >= buffer.GetWidth();
}

void GameObject::ReverseHorizontalDirection(void) { m_xDir *= -1; }
void GameObject::MoveDownBySpriteHeight(void)     { m_yPos = std::max(0, m_yPos - GetHeight()); }

// GameObject PRIVATE METHODS
void GameObject::SetDirectionHorizontal(DirectionHorizontal direction)
{
    m_xDir = static_cast<int32_t>(direction);
}

void GameObject::SetDirectionVertical(DirectionVertical direction)
{
    m_yDir = static_cast<int32_t>(direction);
}

void GameObject::move(int32_t xBound, int32_t yBound)
{
    m_xPos = std::min(xBound - GetWidth() , std::max(0, m_xPos + (m_xDir * m_vel)));
    m_yPos = std::min(yBound - GetHeight(), std::max(0, m_yPos + (m_yDir * m_vel)));
}

GameObject::DirectionHorizontal operator+(const GameObject::DirectionHorizontal & lhs, const GameObject::DirectionHorizontal & rhs)
{
    constexpr GameObject::DirectionHorizontal dirHorMap[] = {
        GameObject::DirectionHorizontal::LEFT,
        GameObject::DirectionHorizontal::STATIONARY,
        GameObject::DirectionHorizontal::RIGHT
    };
    using T = std::underlying_type_t<GameObject::DirectionHorizontal>;
    T result = static_cast<T>(lhs) + static_cast<T>(rhs) + 1;
    assert(0 <= result && result <= 2);
    return dirHorMap[result];
}
