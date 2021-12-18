#ifndef GAME_H
#define GAME_H

#include "global.h"
#include "Size.h"
#include "Buffer.h"
#include "GameObject.h"

#include <list>
#include <memory>

class Game: public Size
{
public:
    Game(int32_t gamewWidth, int32_t gameHeight);
    Game(const Game& other) = delete;

    void    Update(const Buffer & buffer);

    void    Draw(Buffer & buffer)          const;
    int32_t GetPlayerScore(void)           const;

    void create_player_bullet(void);

private:
    const int32_t SCORE_BASE_VAL = 10;

    int32_t m_alien_rows;
    int32_t m_alien_cols;
    int32_t m_playerBulletsMax;
    int32_t m_playerBulletsBonus;
    int32_t m_playerScore;
    bool    m_aliensShouldTurn;

    std::unique_ptr<GameObject>            m_player;
    std::list<std::unique_ptr<GameObject>> m_aliens;
    std::list<std::unique_ptr<GameObject>> m_playerBullets;
    std::list<std::unique_ptr<GameObject>> m_alienBullets;

private:
    void init_aliens(void);
    void create_alien_bullet(const GameObject & alien);
    void update_player(const Buffer & buffer);
    void update_bullets(const Buffer & buffer);
    void update_player_bullets(const Buffer & buffer);
    void update_alien_bullets(const Buffer & buffer);
    void update_aliens(const Buffer & buffer);
    void check_handle_collissions(GameObject & object, std::list<std::unique_ptr<GameObject>> & collection,
                                  bool eraseFromCollection);

    size_t playerMaxAllowedBullets(void) const;
};

#endif
