#ifndef GAME_H
#define GAME_H

#include <vector>
#include <list>

#include "global.h"

#include "Size.h"
#include "Spaceobject.h"

class Game: public Size
{
    public:
        int32_t alien_rows;
        int32_t alien_cols;
        int32_t alien_move_dir;
        Player player;
        std::vector<Spaceobject> aliens;
        std::list<Spaceobject> bullets;

        Game(int32_t, int32_t, Sprites &);
        void create_player_bullet();
        void create_bullet(int32_t, int32_t, Sprite&);
        void update_player(void);
        void update_bullets(void);
    
    private:
        Sprites & _sprites;
        void init_aliens(Sprites &);
};

#endif
