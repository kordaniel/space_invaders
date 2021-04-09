#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include "Size.h"
#include "Position.h"
#include "Sprites.h"
#include "Buffer.h"
#include "Spaceobject.h"


class Game: public Size
{
    public:
        Game(size_t width, size_t height, Sprites &sprites):
            Size(width, height),
            player(Spaceobject(100,200, sprites.player_sprite))
        {
            //
        }
        ~Game(void)
        {
            //
        }

    //private:
        Spaceobject player;
        //std::vector<Spaceobject> objects;
};


int main(void)
{
    const size_t buff_width = 224, buff_height = 256;
    Sprites sprites;
    Buffer buffer(buff_width, buff_height);

    Game game(buff_width, buff_height, sprites);

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
        buffer.append_object(game.player);
        buffer.draw();
        //printf("Player: x: %zu y: %zu\n", game.player.x, game.player.y);
        //printf("Player: w: %zu h: %zu\n" ,game.player.obj_sprite.width, game.player.obj_sprite.height);
        glfwPollEvents();

    }

    /////printf("X: %zu, Y: %zu\n", game.player.x, game.player.y);
    /////printf("Game width: %zu, height %zu\n", game.width, game.height);
    return EXIT_SUCCESS;
}
