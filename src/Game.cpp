#include "Game.h"

Game::Game(int32_t gameWidth, int32_t gameHeight, const Sprites& sprites)
        : Size(gameWidth, gameHeight)
        , m_alien_rows(5)
        , m_alien_cols(11)
        , m_player((width / 2) - (sprites.player_sprite.width / 2), 30, sprites.player_sprite.width, sprites.player_sprite.height)
        , m_sprites(sprites)
        , m_aliensShouldTurn(false)
{
    init_aliens(sprites);
}

void Game::init_aliens(const Sprites& sprites)
{
    // TODO: Move alienTypesMap somewehere "where it belongs"..
    const SpaceobjectType alienTypesMap[] = {
        ALIEN_A, ALIEN_B, ALIEN_C
    };

    for (int32_t yi = 0; yi < m_alien_rows; ++yi) {
        const size_t alien_type   = (m_alien_rows - yi) / 2;
        assert(0 <= alien_type && alien_type <= 2);
        const int32_t alienWidth  = sprites.alien_sprites[2 * alien_type].width;
        const int32_t alienHeight = sprites.alien_sprites[2 * alien_type].height;
        for (int32_t xi = 0; xi < m_alien_cols; ++xi) {
            const size_t xpos = 24 + 16 * xi + (13 - alienWidth) / 2;
            const size_t ypos = 128 + 17 * yi;
            m_aliens.emplace_front(xpos, ypos, directions::LEFT, directions::STATIONARY, alienWidth, alienHeight, 1, 3, alienTypesMap[alien_type]);
        }
    }
}

void Game::create_player_bullet(void)
{
    create_bullet(
        m_player.GetMiddleX(), m_player.GetTopMostY(),
        m_sprites.player_bullet_sprite.width, m_sprites.player_bullet_sprite.height,
        BULLET_PLAYER
    );
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

void Game::updateGame(void)
{
    update_player();
    update_aliens();
    update_bullets();
}

void Game::update_player(void)
{
    m_player.move(0, width, height, 0);
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

        for (auto alienBullerPtr = m_alienBullets.begin(); alienBullerPtr != m_alienBullets.end(); ) {
            if (bulletptr->overlaps(*alienBullerPtr)) {
                m_alienBullets.erase(alienBullerPtr);
                bulletptr = m_playerBullets.erase(bulletptr);
                goto PLAYERBULLETLOOPSTART;
                throw "USE OF GOTO STATEMENTS ARE CONSIDERED BAD AND IS HIGHLY DISCOURAGED";
                // Dont take it too seriously, this is a hobby project after all to learn C++...
            } else ++alienBullerPtr;
        }

        for (auto alienptr = m_aliens.begin(); alienptr != m_aliens.end(); ) {
            if (bulletptr->overlaps(*alienptr)) {
                m_aliens.erase(alienptr);
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
                // Dont take it too seriously, this is a hobby project after all to learn C++...
            } else ++playerBulletPtr;
        }

        if (alienBulletPtr->overlaps(m_player)) {
            io::print_to_stdout("player DIED!!! OHNOOO");
            alienBulletPtr = m_alienBullets.erase(alienBulletPtr);
            goto ALIENBULLETLOOPSTART;
            throw "USE OF GOTO STATEMENTS ARE CONSIDERED BAD AND IS HIGHLY DISCOURAGED";
        }

        if (alienBulletPtr->y == 0) {
            alienBulletPtr = m_alienBullets.erase(alienBulletPtr);
        } else ++alienBulletPtr;
    }
}

void Game::update_aliens(void)
{
    if (m_aliensShouldTurn) {
        for (auto &alien : m_aliens) {
            alien.y -= alien.height;
            alien.ReverseDirection();
            alien.move(0, width, height, 0);
        }
        m_aliensShouldTurn = false;
    } else {
        for (auto &alien : m_aliens) {
            if (alien.move(0, width, height, 0)) {
                m_aliensShouldTurn = true;
            }
            if (rand() % 1000 < 10) {
                create_alien_bullet(alien);
            }
        }
    }
}

Player& Game::getPlayer(void)
{
    return m_player;
}

std::list<Alien>& Game::getAliens(void)
{
    // TODO: DELETE this method when possible
    return m_aliens;
}

std::list<Spaceobject>& Game::getPlayerBullets(void)
{
    // TODO: DELETE this method when possible
    return m_playerBullets;
}

std::list<Spaceobject>& Game::getAlienBullets(void)
{
    // TODO: DELETE this method when possible
    return m_alienBullets;
}
