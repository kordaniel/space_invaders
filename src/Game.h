#ifndef GAME_H
#define GAME_H

#include "global.h"
#include "Size.h"
#include "Sprites.h"
#include "Spaceobject.h"

#include <vector>
#include <list>


class Game: public Size
{
public:
    Game(int32_t gamewWidth, int32_t gameHeight);
    Game(const Game& other) = delete;
    void updateGame(void);
    void create_player_bullet(void); // This is called from glfwwindow callback (for now)
    Player& getPlayer(void) const;
    std::list<Alien>& getAliens(void) const;
    std::list<Spaceobject>& getPlayerBullets(void) const;
    std::list<Spaceobject>& getAlienBullets(void) const;
    int32_t GetPlayerScore(void) const;

private:
    const int32_t SCORE_BASE_VAL = 10;

    int32_t m_alien_rows;
    int32_t m_alien_cols;
    bool m_aliensShouldTurn;
    size_t m_playerBulletsMax;
    size_t m_playerBulletsBonus;
    int32_t m_playerScore;
    const Sprites & m_sprites;
    mutable Player m_player;
    mutable std::list<Alien> m_aliens;
    mutable std::list<Spaceobject> m_playerBullets;
    mutable std::list<Spaceobject> m_alienBullets;

    void init_aliens(const Sprites& sprites);
    void create_alien_bullet(const Alien& alien);
    void create_bullet(int32_t xPosition, int32_t yPosition, int32_t width, int32_t height, SpaceobjectType objectType);
    void update_player(void);
    void update_bullets(void);
    void update_player_bullets(void);
    void update_alien_bullets(void);
    void update_aliens(void);
    size_t playerMaxAllowedBullets(void);
};

#endif
