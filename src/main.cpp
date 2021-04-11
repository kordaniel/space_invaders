#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include "Io.h"
#include "Size.h"
#include "Position.h"
#include "Sprites.h"
#include "Spaceobject.h"
#include "Buffer.h"
#include "Game.h"


int main(void)
{
    const size_t buff_width = 224, buff_height = 256;
    Sprites sprites;
    Buffer buffer(buff_width, buff_height);
    Game game(buff_width, buff_height, sprites);

    glfwSetWindowUserPointer(buffer.get_glfw_window(), &game);
/*
    uint8_t r = (uint8_t) 0;
    uint8_t g = (uint8_t) 0;
    uint8_t b = (uint8_t) 0;
    uint8_t a = (uint8_t) 255;
    //uint32_t clear_color = 0;
*/
    srand(time(NULL));
    size_t x = 0, y = 0, count = 0, bullets = 0;
    while (!glfwWindowShouldClose(buffer.get_glfw_window())) {
        //printf("RUNNING\n");
  //      r+=1;
  //      g+=2;
  //      b+=3;
  //      buffer.clear((uint32_t) (r << 24) | (g << 16) | (b << 8) | a);

/* CREATE ONE BULLET PER SEC
        if (++count % 60 == 0) {
            game.create_bullet(x, y, sprites.player_bullet_sprite);
            game.create_bullet(x, buff_height - y - sprites.alien_bullet_sprites[0].height, sprites.alien_bullet_sprites[0]);
            x += 5;
            y += 8;
        }
*/

/* RANDOMLY DELETE RANDOM BULLET AND DRAW THE REST
        bullets = 0;
        //std::list<Spaceobject>::iterator
        for (auto it = game.bullets.begin(); it != game.bullets.end(); ++it) {
            ++bullets;
            if (count % 60 == 0 && rand() % 100 < 10) {
                --bullets;
                game.bullets.erase(it);
                continue;
            }
            buffer.append_object(*it);
        }
*/
        /* DELETE IN WHILELOOP
        auto it = game.bullets.begin();
        while (it != game.bullets.end()) {
            ++bullets;

            if (count % 60 == 0 && rand() % 100 < 10) {
                --bullets;
                it = game.bullets.erase(it);
                continue;
            }

            buffer.append_object(*it);
            ++it;
        }
        */
        //io::print_to_stdout_varargs("AFTER:", bullets);
        //game.player.move();
        game.update_player();
        buffer.append_object(game.player);
        for (auto &alien : game.aliens) {
            buffer.append_object(alien);
        }

        /*
        int bullets_check = 0;
        for (auto &bullet : game.bullets) {
            ++bullets_check;
            //buffer.append_object(bullet);
        }
        if (bullets != bullets_check) {
            std::cout << "EEEEEEEERRRRRRROOOOOOORRRRRRR!!!!!!!" << std::endl;
        } else {
            //std::cout << "TOIMII.com" << std::endl;
        }
*/


        buffer.draw();
        buffer.clear();
        //printf("Player: x: %zu y: %zu\n", game.player.x, game.player.y);
        //printf("Player: w: %zu h: %zu\n" ,game.player.obj_sprite.width, game.player.obj_sprite.height);
        glfwPollEvents();

    }

    // CLEANUP, yes or no?? nono!
    //glfwSetWindowUserPointer(buffer.get_glfw_window(), nullptr);


    /////printf("X: %zu, Y: %zu\n", game.player.x, game.player.y);
    /////printf("Game width: %zu, height %zu\n", game.width, game.height);
    return EXIT_SUCCESS;
}
