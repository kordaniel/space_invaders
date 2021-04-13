#include "Game.h"

Game::Game(int32_t width, int32_t height, Sprites &sprites):
        Size(width, height),
        alien_rows(5),
        alien_cols(11),
        alien_move_dir(0),
        player(Spaceobject(width / 2 - 5, 30, sprites.player_sprite))
{
    init_aliens(sprites);
}

void Game::init_aliens(Sprites &sprites)
{
    aliens.reserve(alien_rows * alien_cols);

    for (size_t yi = 0; yi < alien_rows; ++yi) {
        const size_t alien_type = (alien_rows - yi) / 2 + 1;

        for (size_t xi = 0; xi < alien_cols; ++xi) {
            Sprite& alien_sprite = sprites.alien_sprites[2 * alien_type - 1];
            const size_t xpos = 24 + 16 * xi + (13 - alien_sprite.width)/2;
            const size_t ypos = 128 + 17 * yi;
            aliens.emplace_back(xpos, ypos, alien_sprite);
        }
    }
}

void Game::create_bullet(int32_t x_pos, int32_t y_pos, Sprite& sprite)
{
    bullets.emplace_front(x_pos, y_pos, sprite);
}

void Game::update_player(void)
{
    player.move(0, width, height, 0);
}
