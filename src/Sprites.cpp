#include "Sprites.h"

Sprite::Sprite(int32_t width, int32_t height, const uint8_t* sprite_data):
    Size(width, height),
    data(sprite_data)
{
    //
}

const uint8_t* Sprite::getSpritePtr(int32_t offset)
{
    const uint8_t* copy = data + offset;
    return copy;
}

const uint8_t* Sprite::getNumberSpritePtr(int32_t number)
{
    return getSpritePtr(16 * getTotalSize() + number * getTotalSize());
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
