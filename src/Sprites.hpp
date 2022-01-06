#ifndef SPRITES_H
#define SPRITES_H

#include "Global.hpp"
#include "Size.hpp"

#include <vector>


enum class GameObjectType;

class Sprite: public Size
{
public:
    uint8_t* m_data;
    Sprite(const std::string& name);
    Sprite(const Sprite & other) = delete; // Should never be called!
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
    const std::vector<Sprite *> m_sprites_player;
    const std::vector<Sprite *> m_sprites_player_bullets;
    const std::vector<Sprite *> m_sprites_alien_deaths;
    const std::vector<Sprite *> m_sprites_alien_bullets;
    const std::vector<std::vector<Sprite *>> m_sprites_aliens;

    Sprite text_spritesheet;

    Sprites(const Sprites& other) = delete;
    ~Sprites(void);
    void operator=(const Sprites&) = delete;

    static Sprites& GetInstance(void);
    const std::vector<Sprite *> * GetSpriteAlive(const GameObjectType objectType) const;
    const std::vector<Sprite *> * GetSpriteDead(const GameObjectType objectType) const;
    const std::vector<Sprite *> & GetSprite(const GameObjectType objectType) const;

private:
    Sprites(void);
};

#endif
