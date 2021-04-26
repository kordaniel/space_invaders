#include "Game.h"

Game::Game(int32_t width, int32_t height, Sprites &sprites)
        : Size(width, height)
        , alien_rows(5)
        , alien_cols(11)
        , player(width / 2 - 5, 30, sprites.player_sprite)
        , _sprites(sprites)
        , m_aliensShouldTurn(false)
{
    init_aliens(sprites);
}

void Game::init_aliens(Sprites &sprites)
{
    for (int32_t yi = 0; yi < alien_rows; ++yi) {
        const size_t alien_type = (alien_rows - yi) / 2 + 1;
        assert(1 <= alien_type && alien_type <= 3);
        for (int32_t xi = 0; xi < alien_cols; ++xi) {
            Sprite& alien_sprite = sprites.alien_sprites[2 * alien_type - 1];
            //io::print_to_stdout_varargs("INIT: ", (2 * alien_type - 1));
            const size_t xpos = 24 + 16 * xi + (13 - alien_sprite.width)/2;
            const size_t ypos = 128 + 17 * yi;
            aliens.emplace_front(xpos, ypos, directions::LEFT, directions::STATIONARY, 1, 3, alien_type, alien_sprite);
        }
    }
}

void Game::create_player_bullet()
{
    create_bullet(player.GetMiddleX(), player.GetTopMostY(), _sprites.player_bullet_sprite);
}

void Game::create_bullet(int32_t x_pos, int32_t y_pos, Sprite& sprite)
{
    bullets.emplace_front(x_pos, y_pos, directions::STATIONARY, directions::UP, 3, 1,
                          sprite, SpaceobjectType::BULLET_PLAYER);
}

void Game::updateGame(void)
{
    update_player();
    update_aliens();
    update_bullets();
}

void Game::update_player(void)
{
    player.move(0, width, height, 0);
}

void Game::update_bullets(void)
{
    for (auto bulletptr = bullets.begin(); bulletptr != bullets.end(); ) {
        bulletptr->move(0, width, height, 0);
        if (bulletptr->GetTopMostY() == height) {
            bulletptr = bullets.erase(bulletptr);
            continue;
        }

        for (auto alienptr = aliens.begin(); alienptr != aliens.end(); ) {
            if (bulletptr->overlaps(*alienptr)) {
                aliens.erase(alienptr);
                bulletptr = bullets.erase(bulletptr);
                break;
            } else ++alienptr;
        }
        ++bulletptr;
    }
}

void Game::update_aliens(void)
{
    if (m_aliensShouldTurn) {
        for (auto &alien : aliens) {
            alien.y -= alien.height;
            alien.ReverseDirection();
            alien.move(0, width, height, 0);
        }
        m_aliensShouldTurn = false;
    } else {
        for (auto &alien : aliens) {
            if (alien.move(0, width, height, 0))
                m_aliensShouldTurn = true;
        }
    }
}

Player& Game::getPlayer(void)
{
    return player;
}

std::list<Alien>& Game::getAliens(void)
{
    // TODO: DELETE this method when possible
    return aliens;
}

std::list<Spaceobject>& Game::getBullets(void)
{
    // TODO: DELETE this method when possible
    return bullets;
}
