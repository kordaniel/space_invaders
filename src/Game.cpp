#include "Game.h"

Game::Game(int32_t gameWidth, int32_t gameHeight, const Sprites& sprites)
        : Size(gameWidth, gameHeight)
        , m_alien_rows(5)
        , m_alien_cols(11)
        , m_aliensShouldTurn(false)
        , m_playerBulletsMax(3)
        , m_playerBulletsBonus(0)
        , m_sprites(sprites)
        , m_player((width / 2) - (sprites.player_sprite.width / 2), 30, sprites.player_sprite.width, sprites.player_sprite.height)
{
    init_aliens(sprites);
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
            m_sprites.player_bullet_sprite.width, m_sprites.player_bullet_sprite.height,
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
        assert(0 <= alien_type && alien_type <= 2);
        const int32_t alienWidth  = sprites.alien_sprites[2 * alien_type].width;
        const int32_t alienHeight = sprites.alien_sprites[2 * alien_type].height;
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
        alien.GetMiddleX() - m_sprites.alien_bullet_sprites[0].width / 2, alien.y,
        m_sprites.alien_bullet_sprites[0].width, m_sprites.alien_bullet_sprites[0].height,
        BULLET_ALIEN
    );
}

void Game::create_bullet(int32_t xPosition, int32_t yPosition, int32_t width, int32_t height, SpaceobjectType objectType)
{
    if (objectType == BULLET_ALIEN) {
        m_alienBullets.emplace_front(xPosition, yPosition, directions::STATIONARY, directions::DOWN, width, height, 4, 1, objectType, true, 1);
        return;
    }
    m_playerBullets.emplace_front(xPosition, yPosition, directions::STATIONARY, directions::UP, width, height, 7, 1, objectType, false, 0);
}



void Game::update_player(void)
{
    m_player.move(0, width, height, 0);
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
        bulletptr->move(0, width, height, 0);

        for (auto alienBulletPtr = m_alienBullets.begin(); alienBulletPtr != m_alienBullets.end(); ) {
            if (bulletptr->overlaps(*alienBulletPtr)) {
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
                alienptr->m_spriteType = ALIEN_DEAD;
                alienptr->lives = 0;
                bulletptr = m_playerBullets.erase(bulletptr);
                goto PLAYERBULLETLOOPSTART;
                throw "USE OF GOTO STATEMENTS ARE CONSIDERED BAD AND IS HIGHLY DISCOURAGED";
            } else ++alienptr;
        }

        if (bulletptr->GetTopMostY() == height) {
            bulletptr = m_playerBullets.erase(bulletptr);
        } else ++bulletptr;
    }
}

void Game::update_alien_bullets(void)
{
    for (auto alienBulletPtr = m_alienBullets.begin(); alienBulletPtr != m_alienBullets.end(); ) {
        ALIENBULLETLOOPSTART:
        alienBulletPtr->move(0, width, height, 0);

        for (auto playerBulletPtr = m_playerBullets.begin(); playerBulletPtr != m_playerBullets.end(); ) {
            if (alienBulletPtr->overlaps(*playerBulletPtr)) {
                m_playerBullets.erase(playerBulletPtr);
                alienBulletPtr = m_alienBullets.erase(alienBulletPtr);
                goto ALIENBULLETLOOPSTART;
                throw "USE OF GOTO STATEMENTS ARE CONSIDERED BAD AND IS HIGHLY DISCOURAGED";
            } else ++playerBulletPtr;
        }

        if (alienBulletPtr->overlaps(m_player)) {
            io::print_to_stdout("player DIED!!! OHNOOO");
            --m_player.lives;
            alienBulletPtr = m_alienBullets.erase(alienBulletPtr);
            goto ALIENBULLETLOOPSTART;
            throw "USE OF GOTO STATEMENTS ARE CONSIDERED BAD AND IS HIGHLY DISCOURAGED";
        }

        if (alienBulletPtr->verticalProjectionOverlaps(m_player)) {
            ++m_playerBulletsBonus;
        }

        if (alienBulletPtr->y == 0) {
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
                alienPtr->y -= alienPtr->height;
                alienPtr->ReverseDirection();
            }
            if (alienPtr->move(0, width, height, 0)) {
                alienSwarmHitWall = true;
            }
            if (rand() % 5000 < ((aN * 10000) / (aN*aN*10))) {
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
