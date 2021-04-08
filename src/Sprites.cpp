#include "Sprites.h"

Sprite::Sprite(size_t width, size_t height, const uint8_t* sprite_data):
    Size(width, height),
    data(sprite_data)
{
    //
}

size_t Sprite::get_length(void)
{
    return width * height;
}

Sprites::Sprites(void):
    player_sprite(Sprite(11, 7, _PLAYER_SPRITE)),
    player_bullet_sprite(Sprite(1, 3, _PLAYER_BULLET_SPRITE)),
    alien_death_sprite(Sprite(13, 7, _ALIEN_DEATH_SPRITE)),
    alien_sprites({
        Sprite(8, 8, _ALIEN_SPRITE_0),
        Sprite(8, 8, _ALIEN_SPRITE_1),
        Sprite(11, 8, _ALIEN_SPRITE_2),
        Sprite(11, 8, _ALIEN_SPRITE_3),
        Sprite(12, 8, _ALIEN_SPRITE_4),
        Sprite(12, 8, _ALIEN_SPRITE_5),
    }),
    alien_bullet_sprites({
        Sprite(3, 7, _ALIEN_BULLET_SPRITE_0),
        Sprite(3, 7, _ALIEN_BULLET_SPRITE_1)
    }),
    text_spritesheet(Sprite(5, 7, _TEXT_SPRITESHEET))
{
    //
}