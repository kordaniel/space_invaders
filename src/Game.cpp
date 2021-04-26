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
    create_bullet(m_player.GetMiddleX(), m_player.GetTopMostY(), m_sprites.player_bullet_sprite);
}

void Game::create_bullet(int32_t xPosition, int32_t yPosition, const Sprite& sprite)
{
    m_bullets.emplace_front(xPosition, yPosition, directions::STATIONARY, directions::UP,
                          sprite.width, sprite.height, 9, 1, SpaceobjectType::BULLET_PLAYER
    );
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
    for (auto bulletptr = m_bullets.begin(); bulletptr != m_bullets.end(); ) {
        bulletptr->move(0, width, height, 0);
        if (bulletptr->GetTopMostY() == height) {
            bulletptr = m_bullets.erase(bulletptr);
            continue;
        }

        for (auto alienptr = m_aliens.begin(); alienptr != m_aliens.end(); ) {
            if (bulletptr->overlaps(*alienptr)) {
                m_aliens.erase(alienptr);
                bulletptr = m_bullets.erase(bulletptr);
                break;
            } else ++alienptr;
        }
        ++bulletptr;
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
            if (alien.move(0, width, height, 0))
                m_aliensShouldTurn = true;
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

std::list<Spaceobject>& Game::getBullets(void)
{
    // TODO: DELETE this method when possible
    return m_bullets;
}
