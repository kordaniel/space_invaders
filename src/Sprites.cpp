#include "Sprites.h"

Sprite::Sprite(int32_t width, int32_t height, const uint8_t* sprite_data):
    Size(width, height),
    data(sprite_data)
{
    //
}

const uint8_t* Sprite::getSpritePtr(int32_t offset) const
{
    const uint8_t* copy = data + offset;
    return copy;
}

const uint8_t* Sprite::getNumberSpritePtr(int32_t number) const
{
    return getSpritePtr(16 * getTotalSize() + number * getTotalSize());
}

Sprites::Sprites(void):
    player_sprite(11, 7, _PLAYER_SPRITE),
    player_bullet_sprite(1, 3, _PLAYER_BULLET_SPRITE),
    alien_death_sprite(13, 7, _ALIEN_DEATH_SPRITE),
    alien_sprites{{
        {8, 8, _ALIEN_SPRITE_0},
        {8, 8, _ALIEN_SPRITE_1},
        {11, 8, _ALIEN_SPRITE_2},
        {11, 8, _ALIEN_SPRITE_3},
        {12, 8, _ALIEN_SPRITE_4},
        {12, 8, _ALIEN_SPRITE_5}
    }},
    alien_bullet_sprites{{
        { 3, 7, _ALIEN_BULLET_SPRITE_0 }, // alien_bullet_sprites[0]
        { 3, 7, _ALIEN_BULLET_SPRITE_1 }  // alien_bullet_sprites[1]
    }},
    text_spritesheet(5, 7, _TEXT_SPRITESHEET)
{
    //
}
