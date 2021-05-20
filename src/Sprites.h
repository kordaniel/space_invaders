#ifndef SPRITES_H
#define SPRITES_H

#include "global.h"

#include "Size.h"
#include "Spaceobject.h"
#include "Compression.h"
#include "Io.h"

#include <array>

class SpaceobjectTypeSpriteSelector; // from Spaceobject.{h,cpp}, must be forward declared

class Sprite: public Size
{
public:
    uint8_t* m_data;
    Sprite(const std::string& name);
    Sprite(const Sprite& other) = delete;
    ~Sprite(void);
    const uint8_t* getSpritePtr(size_t offset) const;
    const uint8_t* getNumberSpritePtr(int32_t number) const;
    bool operator[](size_t index) const;
private:
    const std::string m_name;
    size_t m_count; // how many sprites this sprite(sheet) holds, normal sprites == 1, spritesheets > 1
    void init(void);
    void serialize(void);
    void deserialize(const std::vector<uint8_t>& spriteDataBitmap);
};

class Sprites
{
public:
    Sprite player_sprite;
    Sprite player_bullet_sprite;

    Sprite alien_death_sprite;
    std::array<Sprite, 6> alien_sprites;        // TODO: Move num of alien_sprites &
    std::array<Sprite, 2> alien_bullet_sprites; //       alien_bullet_sprites ..SOMEWHERE..? !!!

    Sprite text_spritesheet;

    Sprites(const Sprites& other) = delete;
    void operator=(const Sprites&) = delete;

    static Sprites& GetInstance(void);
    const Sprite& getSprite(const SpaceobjectType& spriteType,
                            SpaceobjectTypeSpriteSelector& spriteSelector) const;

private:
    Sprites(void);
};

#endif
