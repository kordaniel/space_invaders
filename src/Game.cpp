#include "Game.h"

Game::Game(int32_t gameWidth, int32_t gameHeight)
        : Size(gameWidth, gameHeight)
        , m_alien_rows(5)
        , m_alien_cols(11)
        , m_aliensShouldTurn(false)
        , m_playerBulletsMax(3)
        , m_playerBulletsBonus(0)
        , m_playerScore(0)
        , m_sprites(Sprites::GetInstance())
        , m_player((gameWidth / 2) - (Sprites::GetInstance().player_sprite.GetWidth() / 2), 30, Sprites::GetInstance().player_sprite.GetWidth(), Sprites::GetInstance().player_sprite.GetHeight())
{
    init_aliens(Sprites::GetInstance());
}

void Game::updateGame(void)
{
    update_player();
    update_aliens();
    update_bullets();
}

void Game::create_player_bullet(void)
{
    if (m_playerBullets.size() < playerMaxAllowedBullets()) {
        create_bullet(
            m_player.GetMiddleX(), m_player.GetTopMostY(),
            m_sprites.player_bullet_sprite.GetWidth(), m_sprites.player_bullet_sprite.GetHeight(),
            BULLET_PLAYER
        );
    }
}

Player& Game::getPlayer(void) const
{
    return m_player;
}

std::list<Alien>& Game::getAliens(void) const
{
    // TODO: DELETE this method when possible
    return m_aliens;
}

std::list<Spaceobject>& Game::getPlayerBullets(void) const
{
    // TODO: DELETE this method when possible
    return m_playerBullets;
}

std::list<Spaceobject>& Game::getAlienBullets(void) const
{
    // TODO: DELETE this method when possible
    return m_alienBullets;
}

int32_t Game::GetPlayerScore(void) const
{
    return m_playerScore;
}


// PRIVATE METHODS
void Game::init_aliens(const Sprites& sprites)
{
    // TODO: Move alienTypesMap somewehere "where it belongs"..
    const SpaceobjectType alienTypesMap[] = {
        ALIEN_A, ALIEN_B, ALIEN_C
    };
    directions::Directions initialDirection = rand() % 2 == 0
                                            ? directions::LEFT : directions::RIGHT;
    const int32_t initalSpeed = 1;
    for (int32_t yi = 0; yi < m_alien_rows; ++yi) {
        const size_t alien_type   = (m_alien_rows - yi) / 2;
        assert(alien_type <= 2);
        const int32_t alienWidth  = sprites.alien_sprites[2 * alien_type].GetWidth();
        const int32_t alienHeight = sprites.alien_sprites[2 * alien_type].GetHeight();
        for (int32_t xi = 0; xi < m_alien_cols; ++xi) {
            const size_t xpos = 24 + 16 * xi + (13 - alienWidth) / 2;
            const size_t ypos = 128 + 17 * yi;
            m_aliens.emplace_front(
                xpos, ypos, initialDirection, directions::STATIONARY,
                alienWidth, alienHeight, initalSpeed, 1, alienTypesMap[alien_type]
            );
        }
    }
}



void Game::create_alien_bullet(const Alien& alien)
{
    create_bullet(
        alien.GetMiddleX() - m_sprites.alien_bullet_sprites[0].GetWidth() / 2, alien.GetY(),
        m_sprites.alien_bullet_sprites[0].GetWidth(), m_sprites.alien_bullet_sprites[0].GetHeight(),
        BULLET_ALIEN
    );
}

void Game::create_bullet(int32_t xPosition, int32_t yPosition, int32_t width, int32_t height,
                         SpaceobjectType objectType)
{
    if (objectType == BULLET_ALIEN) {
        m_alienBullets.emplace_front(
            xPosition, yPosition, directions::STATIONARY, directions::DOWN,
            width, height,
            6, 1, objectType, true, 1
        );
        return;
    }

    m_playerBullets.emplace_front(
        xPosition, yPosition, directions::STATIONARY, directions::UP,
        width, height, 7, 1, objectType, false, 0
    );
}



void Game::update_player(void)
{
    m_player.move(0, m_width, m_height, 0);
    m_playerBulletsBonus = 0;
}

void Game::update_bullets(void)
{
    //ScopeTimer t("Bullet updating took: ");
    update_alien_bullets();
    update_player_bullets();
}

void Game::update_player_bullets(void)
{
    for (auto bulletptr = m_playerBullets.begin(); bulletptr != m_playerBullets.end(); ) {
        PLAYERBULLETLOOPSTART: // Return here from inner loop, if bullet is destroyed while iterating other objects.
        bulletptr->move(0, m_width, m_height, 0);

        for (auto alienBulletPtr = m_alienBullets.begin(); alienBulletPtr != m_alienBullets.end(); ) {
            if (bulletptr->overlaps(*alienBulletPtr)) {
                m_playerScore += SCORE_BASE_VAL / 2;
                m_alienBullets.erase(alienBulletPtr);
                bulletptr = m_playerBullets.erase(bulletptr);
                goto PLAYERBULLETLOOPSTART;
                throw "USE OF GOTO STATEMENTS ARE CONSIDERED BAD AND IS HIGHLY DISCOURAGED";
                // Dont take it too seriously, this is a hobby project after all to learn C++...
                // And sometimes we can spare the trouble of dealing with one bool if we use GOTO
            } else ++alienBulletPtr;
        }

        for (auto alienptr = m_aliens.begin(); alienptr != m_aliens.end(); ) {
            if (bulletptr->overlaps(*alienptr) && alienptr->isAlive()) {
                m_playerScore += (3 - alienptr->getSpriteType() / 2) * SCORE_BASE_VAL;
                alienptr->m_spriteType = ALIEN_DEAD;
                alienptr->m_lives = 0;
                bulletptr = m_playerBullets.erase(bulletptr);
                goto PLAYERBULLETLOOPSTART;
                throw "USE OF GOTO STATEMENTS ARE CONSIDERED BAD AND IS HIGHLY DISCOURAGED";
            } else ++alienptr;
        }

        if (bulletptr->GetTopMostY() == m_height) {
            bulletptr = m_playerBullets.erase(bulletptr);
        } else ++bulletptr;
    }
}

void Game::update_alien_bullets(void)
{
    auto alienBulletPtr = m_alienBullets.begin();
    while (true) {
        ALIENBULLETLOOPSTART:
        if (alienBulletPtr == m_alienBullets.end()) {
            break;
        }

        alienBulletPtr->move(0, m_width, m_height, 0);

        for (auto playerBulletPtr = m_playerBullets.begin(); playerBulletPtr != m_playerBullets.end(); ) {
            if (alienBulletPtr->overlaps(*playerBulletPtr)) {
                m_playerScore += SCORE_BASE_VAL / 2;
                m_playerBullets.erase(playerBulletPtr);
                alienBulletPtr = m_alienBullets.erase(alienBulletPtr);
                goto ALIENBULLETLOOPSTART;
                throw "USE OF GOTO STATEMENTS ARE CONSIDERED BAD AND IS HIGHLY DISCOURAGED";
            } else ++playerBulletPtr;
        }

        if (alienBulletPtr->overlaps(m_player)) {
            --m_player.m_lives;
            alienBulletPtr = m_alienBullets.erase(alienBulletPtr);
            goto ALIENBULLETLOOPSTART;
            throw "USE OF GOTO STATEMENTS ARE CONSIDERED BAD AND IS HIGHLY DISCOURAGED";
        }

        if (alienBulletPtr->verticalProjectionOverlaps(m_player)) {
            ++m_playerBulletsBonus;
        }

        if (alienBulletPtr->GetY() == 0) {
            alienBulletPtr = m_alienBullets.erase(alienBulletPtr);
        } else ++alienBulletPtr;
    }
}

void Game::update_aliens(void)
{
    bool alienSwarmHitWall = false;
    size_t aN = m_aliens.size();
    for (auto alienPtr = m_aliens.begin(); alienPtr != m_aliens.end(); ) {
        if ((alienPtr->m_timer) == 0) {
            alienPtr = m_aliens.erase(alienPtr);
            continue;
        }

        if (alienPtr->isAlive()) {
            if (m_aliensShouldTurn) {
                alienPtr->UpdateY(-alienPtr->GetHeight());
                alienPtr->ReverseDirection();
            }
            if (alienPtr->move(0, m_width, m_height, 0)) {
                alienSwarmHitWall = true;
            }
            if ((size_t) rand() % 5000 < ((aN * 10000) / (aN*aN*10))) {
                create_alien_bullet(*alienPtr);
            }
        } else {
            alienPtr->m_timer--;
        }
        ++alienPtr;
    }
    m_aliensShouldTurn = alienSwarmHitWall;
}

size_t Game::playerMaxAllowedBullets(void)
{
    return m_playerBulletsMax + m_playerBulletsBonus;
}
