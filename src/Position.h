#ifndef POSITION_H
#define POSITION_H

#include <cstdlib>

class Position
{
    public:
        Position(size_t, size_t);
        size_t x;
        size_t y;
        int x_d;
        int y_d;
};

#endif
