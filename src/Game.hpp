#ifndef GAME_HPP
#define GAME_HPP

#include "Global.hpp"

#include "Tools.hpp"
#include "Size.hpp"
#include "Buffer.hpp"
#include "GameObject.hpp"

#include <list>
#include <memory>


class Game: public Size
{
public:
    Game(int32_t gamewWidth, int32_t gameHeight);
    Game(const Game& other) = delete;

    void    Run(Buffer & buffer);
    void    Update(const Buffer & buffer, SI::Timetools::Timestep ts);
    void    Draw(Buffer & buffer) const;
    int32_t GetPlayerScore(void)  const;

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
    void create_player_bullet(void);
    void create_alien_bullet(const GameObject & alien);
    void update_player(const Buffer & buffer, SI::Timetools::Timestep ts);
    void update_bullets(const Buffer & buffer, SI::Timetools::Timestep ts);
    void update_player_bullets(const Buffer & buffer, SI::Timetools::Timestep ts);
    void update_alien_bullets(const Buffer & buffer, SI::Timetools::Timestep ts);
    void update_aliens(const Buffer & buffer, SI::Timetools::Timestep ts);
    void check_handle_collissions(GameObject & object, std::list<std::unique_ptr<GameObject>> & collection,
                                  bool eraseFromCollection);

    void update_ups(void);
    size_t playerMaxAllowedBullets(void) const;
};

#endif
