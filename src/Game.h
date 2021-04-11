#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <vector>
#include <list>

#include "Size.h"
#include "Spaceobject.h"

class Game: public Size
{
    public:
        size_t alien_rows;
        size_t alien_cols;
        int alien_move_dir;
        Spaceobject player;
        std::vector<Spaceobject> aliens;
        std::list<Spaceobject> bullets;

        Game(size_t, size_t, Sprites &);
        void create_bullet(size_t, size_t, Sprite&);
        void update_player(void);
    
    private:
        void init_aliens(Sprites &);
};

#endif
