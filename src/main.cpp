#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include "Size.h"
#include "Sprites.h"
#include "Buffer.h"


class Position {
    public:
        size_t x, y;
};



class SpaceObject: public Size, public Position
{
    public:
        SpaceObject(size_t x_pos, size_t y_pos):
            Size(10, 10)
        {
            //width = xx, height = xx
            lifes          = 3;
            move_direction = 0;
            x = x_pos;
            y = y_pos;
        }

        ~SpaceObject(void)
        {
            //
        }

    private:
        size_t lifes;
        int move_direction;
};

class Game: public Size
{
    public:
        Game(size_t width, size_t height):
            Size(width, height),
            player{SpaceObject(100,200)}
        {
            //
        }
        ~Game(void)
        {
            //
        }

    //private:
        SpaceObject player;
        //std::vector<SpaceObject> objects;
};


int main(void)
{
    const size_t buff_width = 224, buff_height = 256;
    Sprites sprites;
    Buffer buffer(buff_width, buff_height);

    //Game game(buff_width, buff_height);

    //SpaceObject player;
    //player = new SpaceObject(100, 100);

    uint8_t r = (uint8_t) 0;
    uint8_t g = (uint8_t) 0;
    uint8_t b = (uint8_t) 0;
    uint8_t a = (uint8_t) 255;
    //uint32_t clear_color = 0;

    while (!glfwWindowShouldClose(buffer.get_glfw_window())) {
        //printf("RUNNING\n");
        r+=1;
        g+=2;
        b+=3;
        buffer.clear((uint32_t) (r << 24) | (g << 16) | (b << 8) | a);
        buffer.draw();

        glfwPollEvents();

    }

    /////printf("X: %zu, Y: %zu\n", game.player.x, game.player.y);
    /////printf("Game width: %zu, height %zu\n", game.width, game.height);
    return EXIT_SUCCESS;
}
