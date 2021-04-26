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
    Game(int32_t, int32_t, Sprites &);
    Game(const Game& other) = delete;
    void updateGame(void);
    void create_player_bullet();
    void update_player(void);
    void update_bullets(void);
    void update_aliens(void);
    Player& getPlayer(void);
    std::list<Alien>& getAliens(void);
    std::list<Spaceobject>& getBullets(void);
    
private:
    int32_t alien_rows;
    int32_t alien_cols;
    Player player;
    Sprites & _sprites;
    bool m_aliensShouldTurn;
    std::list<Alien> aliens;
    std::list<Spaceobject> bullets;
    void init_aliens(Sprites &);
    void create_bullet(int32_t, int32_t, Sprite&);
};

#endif
