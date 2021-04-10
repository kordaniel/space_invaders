#include "Game.h"

Game::Game(size_t width, size_t height, Sprites &sprites):
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

    for (size_t xi = 0; xi < alien_cols; ++xi) {
        for (size_t yi = 0; yi < alien_rows; ++yi) {
            const size_t alien_type = (5-yi) / 2 + 1;

            Sprite& alien_sprite = sprites.alien_sprites[2 * alien_type - 1];
            const size_t xpos = 24 + 16 * xi + (13 - alien_sprite.width)/2;
            const size_t ypos = 128 + 17 * yi;

            aliens.push_back(Spaceobject(xpos, ypos, alien_sprite));
        }
    }
}
