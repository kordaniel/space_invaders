#ifndef SPRITES_H
#define SPRITES_H

#include <vector>

#include "global.h"

#include "Size.h"

class Sprite: public Size
{
    public:
        const uint8_t* data;
        Sprite(int32_t, int32_t, const uint8_t*);
        int32_t get_length(void);
};

class Sprites
{
    private:
        const uint8_t* _PLAYER_SPRITE = (uint8_t[]) {
            // size: 11 * 7 = 77
            0,0,0,0,0,1,0,0,0,0,0, // .....@.....
            0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
            0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
            0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
            1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
            1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
            1,1,1,1,1,1,1,1,1,1,1  // @@@@@@@@@@@
        };
        const uint8_t* _PLAYER_BULLET_SPRITE = (uint8_t[]) { 1,1,1 };
        const uint8_t* _ALIEN_DEATH_SPRITE = (uint8_t[]) {
            // size: 13 * 7 = 91
            0,1,0,0,1,0,0,0,1,0,0,1,0, // .@..@...@..@.
            0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
            0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
            1,1,0,0,0,0,0,0,0,0,0,1,1, // @@.........@@
            0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
            0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
            0,1,0,0,1,0,0,0,1,0,0,1,0  // .@..@...@..@.
        };
        const uint8_t* _ALIEN_SPRITE_0 = (uint8_t[]) {
            // size: 8 * 8 = 64
            0,0,0,1,1,0,0,0, // ...@@...
            0,0,1,1,1,1,0,0, // ..@@@@..
            0,1,1,1,1,1,1,0, // .@@@@@@.
            1,1,0,1,1,0,1,1, // @@.@@.@@
            1,1,1,1,1,1,1,1, // @@@@@@@@
            0,1,0,1,1,0,1,0, // .@.@@.@.
            1,0,0,0,0,0,0,1, // @......@
            0,1,0,0,0,0,1,0  // .@....@.
        };
        const uint8_t* _ALIEN_SPRITE_1 = (uint8_t[]) {
            // size: 8 * 8 = 64
            0,0,0,1,1,0,0,0, // ...@@...
            0,0,1,1,1,1,0,0, // ..@@@@..
            0,1,1,1,1,1,1,0, // .@@@@@@.
            1,1,0,1,1,0,1,1, // @@.@@.@@
            1,1,1,1,1,1,1,1, // @@@@@@@@
            0,0,1,0,0,1,0,0, // ..@..@..
            0,1,0,1,1,0,1,0, // .@.@@.@.
            1,0,1,0,0,1,0,1  // @.@..@.@
        };
        const uint8_t* _ALIEN_SPRITE_2 = (uint8_t[]) {
            // size 11 * 8 = 88
            0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
            0,0,0,1,0,0,0,1,0,0,0, // ...@...@...
            0,0,1,1,1,1,1,1,1,0,0, // ..@@@@@@@..
            0,1,1,0,1,1,1,0,1,1,0, // .@@.@@@.@@.
            1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
            1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
            1,0,1,0,0,0,0,0,1,0,1, // @.@.....@.@
            0,0,0,1,1,0,1,1,0,0,0  // ...@@.@@...
        };
        const uint8_t* _ALIEN_SPRITE_3 = (uint8_t[]) {
            // size 11 * 8 = 88
            0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
            1,0,0,1,0,0,0,1,0,0,1, // @..@...@..@
            1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
            1,1,1,0,1,1,1,0,1,1,1, // @@@.@@@.@@@
            1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
            0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
            0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
            0,1,0,0,0,0,0,0,0,1,0  // .@.......@.
        };
        const uint8_t* _ALIEN_SPRITE_4 = (uint8_t[]) {
            // size 12 * 8 = 96
            0,0,0,0,1,1,1,1,0,0,0,0, // ....@@@@....
            0,1,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@@.
            1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
            1,1,1,0,0,1,1,0,0,1,1,1, // @@@..@@..@@@
            1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
            0,0,0,1,1,0,0,1,1,0,0,0, // ...@@..@@...
            0,0,1,1,0,1,1,0,1,1,0,0, // ..@@.@@.@@..
            1,1,0,0,0,0,0,0,0,0,1,1  // @@........@@
        };
        const uint8_t* _ALIEN_SPRITE_5 = (uint8_t[]) {
            // size 12 * 8 = 96
            0,0,0,0,1,1,1,1,0,0,0,0, // ....@@@@....
            0,1,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@@.
            1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
            1,1,1,0,0,1,1,0,0,1,1,1, // @@@..@@..@@@
            1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
            0,0,1,1,1,0,0,1,1,1,0,0, // ..@@@..@@@..
            0,1,1,0,0,1,1,0,0,1,1,0, // .@@..@@..@@.
            0,0,1,1,0,0,0,0,1,1,0,0  // ..@@....@@..
        };
        const uint8_t* _ALIEN_BULLET_SPRITE_0 = // length: 3 * 7 = 21
            (uint8_t[]) {0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0};
        const uint8_t* _ALIEN_BULLET_SPRITE_1 = // length: 3 * 7 = 21
            (uint8_t[]) {0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0};
        const uint8_t* _TEXT_SPRITESHEET = (uint8_t[]) {
            // length 5 * 7 * 65 = 2275
            // ASCII characters in the range [32,96] == ['space','`']
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,
            0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,1,0,1,0,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,0,1,0,1,0,
            0,0,1,0,0,0,1,1,1,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,0,1,0,0,
            1,1,0,1,0,1,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,1,0,1,0,1,1,
            0,1,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,0,1,0,0,0,1,0,1,1,1,1,
            0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,
            1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,
            0,0,1,0,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,1,0,0,
            0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
            0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,

            0,1,1,1,0,1,0,0,0,1,1,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,0,1,1,1,0,
            0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,0,
            0,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,
            1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
            0,0,0,1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,1,0,
            1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
            0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
            1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,
            0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
            0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,

            0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,
            0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,
            0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
            1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,
            0,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,
            0,1,1,1,0,1,0,0,0,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,0,1,0,0,0,1,0,1,1,1,0,

            0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,
            1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,
            0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,1,1,0,
            1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,
            1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,
            1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,
            0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,0,1,1,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
            1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,
            0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,0,
            0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
            1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,1,0,0,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,1,
            1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,
            1,0,0,0,1,1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,
            1,0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1,
            0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
            1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,
            0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,0,1,1,0,1,1,1,1,
            1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,1,
            0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,0,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,0,
            1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,
            1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
            1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,
            1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,0,0,0,1,
            1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,
            1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,
            1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,

            0,0,0,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,
            0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,
            1,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,0,
            0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
            0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        };

    public:
        Sprite player_sprite;
        Sprite player_bullet_sprite;

        Sprite alien_death_sprite;
        std::vector<Sprite> alien_sprites;
        std::vector<Sprite> alien_bullet_sprites;

        Sprite text_spritesheet;

        Sprites(void);
};

#endif
