#ifndef GAME_H
#define GAME_H

#include "global.h"
#include <vector>
#include <list>

#include "Size.h"
#include "Spaceobject.h"

class Game: public Size
{
public:
    Game(int32_t gamwWidth, int32_t gameHeight, const Sprites& sprites);
    Game(const Game& other) = delete;
    void updateGame(void);
    void create_player_bullet(void);
    void update_player(void);
    void update_bullets(void);
    void update_aliens(void);
    Player& getPlayer(void);
    std::list<Alien>& getAliens(void);
    std::list<Spaceobject>& getPlayerBullets(void);
    std::list<Spaceobject>& getAlienBullets(void);
    
private:
    int32_t m_alien_rows;
    int32_t m_alien_cols;
    Player m_player;
    const Sprites & m_sprites;
    bool m_aliensShouldTurn;
    std::list<Alien> m_aliens;
    std::list<Spaceobject> m_playerBullets;
    std::list<Spaceobject> m_alienBullets;

    void init_aliens(const Sprites& sprites);
    void create_alien_bullet(const Alien& alien);
    void create_bullet(int32_t xPosition, int32_t yPosition, int32_t width, int32_t height,
                       SpaceobjectType objectType);
    void update_player_bullets(void);
    void update_alien_bullets(void);
};

#endif
