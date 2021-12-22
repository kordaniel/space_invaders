#include "Game.h"

#include "Input.h"
#include "Timer.h"

#include <functional>   // std::bind
#include <type_traits>
#include <chrono>
#include <GLFW/glfw3.h>


Game::Game(int32_t gameWidth, int32_t gameHeight)
        : Size(gameWidth, gameHeight)
        , m_alien_rows(5)
        , m_alien_cols(11)
        , m_playerBulletsMax(3)
        , m_playerBulletsBonus(0)
        , m_playerScore(0)
        , m_aliensShouldTurn(false)
        , m_player(GameObject::CreatePlayer(3, gameWidth / 2, 30, 70))
{
    KeyInput::RegisterCallback(
        GLFW_KEY_SPACE,
        GLFW_RELEASE,
        std::bind(&Game::create_player_bullet, this)
    );
    init_aliens();
}

void Game::Run(Buffer & buffer)
{
    // Note: All times are in seconds.

    using namespace std::literals::chrono_literals;

    using Clock      = std::chrono::steady_clock;
    using duration   = std::chrono::duration<double>;
    using time_point = std::chrono::time_point<Clock, duration>;
    using SI::Timetools::Timestep;
    using SI::Timetools::TimeEstimate;

    int32_t targetFPS       = 60;
    int32_t targetUPS       = 2 * targetFPS;    // Game state updates per sec
    auto deltaTimeUpdate    = 1.0s / targetUPS; // Gametime length to update game state
    auto loopIterTargetTime = 1.0s / targetFPS; // Total time for one iteration of the loop

    TimeEstimate oneMsSleepActualTime(0.003, 0.003);
    time_point timeCurrent  = Clock::now();
    time_point timePrevious = Clock::now();
    duration accumulatedLag = 0s;

    Timestep timeToSleep    = 0.0;
    bool drawFrame          = true;
    int32_t framesLagCount  = 0;

    while (!glfwWindowShouldClose(buffer.get_glfw_window()))
    {
        //Timer gameLoopTimer("Game loop: ");

        timeCurrent = Clock::now();
        accumulatedLag += timeCurrent - timePrevious;
        timePrevious = timeCurrent;

        glfwPollEvents();

        while (accumulatedLag >= deltaTimeUpdate)
        {
            Update(buffer, deltaTimeUpdate.count());
            accumulatedLag -= deltaTimeUpdate;
        }

        if (drawFrame) {
            Draw(buffer);
        } else {
#ifndef NDEBUG
            Logger::Debug("Dropping frame! Framelag: %d", framesLagCount);
#else
            Logger::Info("Dropping frame");
#endif
        }

        timeToSleep = (loopIterTargetTime - (time_point{Clock::now()} - timeCurrent)).count();
        if (timeToSleep > 0)
        {
            if (framesLagCount > 0) {
                --framesLagCount;
            }
            drawFrame = true;
            SI::thread::PreciseSleep(timeToSleep, oneMsSleepActualTime);
        }
        else
        {   // TODO: If framesLagCount grows too large or keeps rising
            //       => improve rendering and/or game state update speed,
            //          increase deltaTimeUpdate
            drawFrame = ++framesLagCount % targetFPS == 0
                      ? true : false;
        }
    }
}

void Game::Update(const Buffer & buffer, SI::Timetools::Timestep ts)
{
    update_player(buffer, ts);
    update_aliens(buffer, ts);
    update_bullets(buffer, ts);
    update_ups();
}

void Game::Draw(Buffer & buffer) const
{
#ifndef NDEBUG
    buffer.append_text(GetWidth(), GetHeight() - Sprites::GetInstance().text_spritesheet.GetHeight() - 7, Sprites::GetInstance().text_spritesheet, "ALIEN BULLETS: " + std::to_string(m_alienBullets.size()));
#endif
    buffer.append_text(164, 7, Sprites::GetInstance().text_spritesheet, "CREDIT " + std::to_string(SI::GLOBAL::VERSION));
    buffer.append_text(4, buffer.GetHeight() - Sprites::GetInstance().text_spritesheet.GetHeight() - 7, Sprites::GetInstance().text_spritesheet, "SCORE");
    buffer.append_integer(4 + 2 * Sprites::GetInstance().text_spritesheet.GetWidth(),
                          buffer.GetHeight() - 2 * Sprites::GetInstance().text_spritesheet.GetHeight() - 12,
                          Sprites::GetInstance().text_spritesheet, GetPlayerScore()
    );
    buffer.append_text(4, 7, Sprites::GetInstance().text_spritesheet, std::to_string(m_player->GetLivesAmount()));
    int32_t xpos = 13;
    Sprite * playerSprite = Sprites::GetInstance().GetSprite(GameObjectType::PLAYER)[0];
    for (int32_t i = 1; i < m_player->GetLivesAmount(); ++i)
    {
        buffer.drawSprite(xpos, 7, *playerSprite, colors::ORANGE);
        xpos += playerSprite->GetWidth() + 3;
    }

    m_player->Draw(buffer);
    for (auto & obj : m_aliens) {
        obj->Draw(buffer);
    }
    for (auto & obj : m_alienBullets) {
        obj->Draw(buffer);
    }
    for (auto & obj : m_playerBullets) {
        obj->Draw(buffer);
    }

    buffer.append_horizontal_line(16);

    buffer.draw();
    buffer.clear();
}

int32_t Game::GetPlayerScore(void) const { return m_playerScore; }

// PRIVATE METHODS
void Game::init_aliens(void)
{
    const int32_t initialVelocity = 15;
    const GameObject::DirectionHorizontal initialDirection
            = SI::math::RandomBinary() == 0
            ? GameObject::DirectionHorizontal::LEFT
            : GameObject::DirectionHorizontal::RIGHT;
    for (int32_t yi = 0; yi < m_alien_rows; ++yi)
    {
        const GameObjectType alienType = GameObject::TypesMap[m_alien_rows - yi];
        assert(static_cast<std::underlying_type_t<GameObjectType>>(alienType) <= 5);
        for (int32_t xi = 0; xi < m_alien_cols; ++xi)
        {
            const int32_t xpos = 32  + 16 * xi;
            const int32_t ypos = 128 + 17 * yi;
            m_aliens.emplace_front(
                GameObject::CreateAlien(alienType, 1, xpos, ypos, initialDirection, initialVelocity)
            );
        }
    }
}

void Game::create_player_bullet(void)
{
#ifndef NDEBUG
    if (!m_player) {
        Logger::Debug("Player is nullptr!!");
        return;
    }
#endif

    if (m_playerBullets.size() < playerMaxAllowedBullets())
    {
        m_playerBullets.emplace_front(
            GameObject::CreatePlayerBullet(1, m_player.get()->GetMiddleX(), m_player.get()->GetTopMostY(), 200)
        );
    }

}

void Game::create_alien_bullet(const GameObject & alien)
{
    m_alienBullets.emplace_back( GameObject::CreateAlienBullet(1, alien.GetMiddleX(), alien.GetY(), 100));
}

void Game::update_player(const Buffer & buffer, SI::Timetools::Timestep ts)
{
    if (!m_player) {
        return;
    }
    m_player.get()->Update(buffer, ts);
    m_playerBulletsBonus = 0; // TODO: Refactor!
}

void Game::update_bullets(const Buffer & buffer, SI::Timetools::Timestep ts)
{
    update_alien_bullets(buffer, ts);
    update_player_bullets(buffer, ts);
}

void Game::update_player_bullets(const Buffer & buffer, SI::Timetools::Timestep ts)
{
    for (auto bulletPtr = m_playerBullets.begin(); bulletPtr != m_playerBullets.end(); )
    {
        GameObject & bullet = *bulletPtr->get();
        bullet.Update(buffer, ts);

        check_handle_collissions(bullet, m_alienBullets, true);
        check_handle_collissions(bullet, m_aliens, false);

        if (!bullet.IsAlive() || bullet.GetTopMostY() >= m_height) {
            bulletPtr = m_playerBullets.erase(bulletPtr);
        } else {
            ++bulletPtr;
        }
    }
}

void Game::update_alien_bullets(const Buffer & buffer, SI::Timetools::Timestep ts)
{
    for (auto alienBulletPtr = m_alienBullets.begin(); alienBulletPtr != m_alienBullets.end(); )
    {
        GameObject & alienBullet = *alienBulletPtr->get();
        alienBullet.Update(buffer, ts);

        check_handle_collissions(alienBullet, m_playerBullets, true);

        if (alienBullet.IsAlive())
        {
            if (alienBullet.Overlaps(*m_player))
            {
                alienBullet.HandleHit();
                m_player->HandleHit();
            } else if (alienBullet.VerticalProjectionOverlaps(*m_player)) {
                ++m_playerBulletsBonus;
            }
        }

        if (!alienBullet.IsAlive() || alienBullet.GetY() <= 0) {
            alienBulletPtr = m_alienBullets.erase(alienBulletPtr);
        } else {
            ++alienBulletPtr;
        }
    }
}

void Game::check_handle_collissions(GameObject & object, std::list<std::unique_ptr<GameObject>> & collection,
                                    bool eraseFromCollection)
{
    if (!object.IsAlive()) {
        return;
    }

    for (auto objPtr = collection.begin(); objPtr != collection.end(); ++objPtr)
    {
        GameObject & otherObj = *objPtr->get();
        if (!otherObj.IsAlive()) {
            continue;
        } else if (object.Overlaps(otherObj))
        {
            object.HandleHit();
            otherObj.HandleHit();
            if (eraseFromCollection) {
                // Bullet
                m_playerScore += SCORE_BASE_VAL / 2;    // TODO: Refactor!
                collection.erase(objPtr);
            } else {
                // Alien
                m_playerScore += (3 - otherObj.GetDestroyScore()) * SCORE_BASE_VAL; // TODO: Refactor!
            }

            return;
        }
    }
}

void Game::update_aliens(const Buffer & buffer, SI::Timetools::Timestep ts)
{
    const double aN = static_cast<double>(m_aliens.size());
    const double targetBulletsPerSec = 5.0; // Spawned
    bool alienSwarmHitWall = false;

    for (auto alienPtr = m_aliens.begin(); alienPtr != m_aliens.end(); )
    {
        GameObject & alien = *alienPtr->get();
        if (alien.IsAlive())
        {
            if (m_aliensShouldTurn)
            {
                alien.ReverseHorizontalDirection();
                alien.MoveDownBySpriteHeight();
            }
            if (alien.Update(buffer, ts)) {
                alienSwarmHitWall = true;
            }
            if (SI::math::RandomNormalized() < (targetBulletsPerSec * ts / aN)) {
                create_alien_bullet(alien);
            }
        }
        else
        {
            if (alien.GetDeathCounter() == 0) {
                alienPtr = m_aliens.erase(alienPtr);
                continue;
            } else {
                alien.Update(buffer, ts);
            }
        }
        ++alienPtr;
    }
    m_aliensShouldTurn = alienSwarmHitWall;
}

void Game::update_ups(void)
{
    using namespace std::chrono;
    static size_t updates = 0;
    static time_point<steady_clock> time_prev = steady_clock::now();

    ++updates;
    time_point<steady_clock> time_now = steady_clock::now();
    duration<double> time_delta = time_now - time_prev;

    if (time_delta < seconds(1)) {
        return;
    }

    Logger::Debug("Game   UPS: %.3f", (updates / time_delta.count()));

    time_prev = time_now;
    updates = 0;
}

size_t Game::playerMaxAllowedBullets(void) const {
    assert(m_playerBulletsMax + m_playerBulletsBonus >= 0);
    return static_cast<size_t>(m_playerBulletsMax + m_playerBulletsBonus);
}
