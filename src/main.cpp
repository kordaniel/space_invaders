#include "main.h"
#include <iostream>
#include <cstdlib> // srand, rand
#include <ctime>   // time_t

/*static bool s_Finished = false;

void do_heavyCalculation()
{
    while (!s_Finished) {
        io::print_to_stdout("Huhhuh, so heavy lifting going on");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
} */

int main(void)
{
    srand(time(nullptr));

    const int32_t buff_width = 224, buff_height = 256;
    Sprites sprites;
    Buffer buffer(buff_width, buff_height, sprites);
    Game game(buff_width, buff_height, sprites);

    glfwSetWindowUserPointer(buffer.get_glfw_window(), &game);
/*
    uint8_t r = (uint8_t) 0;
    uint8_t g = (uint8_t) 0;
    uint8_t b = (uint8_t) 0;
    uint8_t a = (uint8_t) 255;
    //uint32_t clear_color = 0;
*/
    // Ugly hack to limit the speed of the game in DEBUG mode for now..
    // **************
    #ifdef DEBUG
    Timer timer;
    uint32_t updatesPerSecHack = 20;
    using std::chrono::time_point;
    using std::chrono::steady_clock;

    time_point<steady_clock> time_prev_update = steady_clock::now();
    time_point<steady_clock> time_now = steady_clock::now();
    #endif

    while (!glfwWindowShouldClose(buffer.get_glfw_window())) {
        #ifdef DEBUG
        time_now = steady_clock::now();
        if ((time_now - time_prev_update) > std::chrono::milliseconds(1000 / updatesPerSecHack)) {
        #endif
            game.updateGame();
        #ifdef DEBUG
            time_prev_update = time_now;
        }
        #endif
        // End ugly hack!
        // **************

        buffer.append_text(164, 7, sprites.text_spritesheet, "CREDIT 00");

        buffer.append_text(4, buff_height - sprites.text_spritesheet.height - 7, sprites.text_spritesheet, "SCORE");
        // Actual score
        buffer.append_text(4 + 2 * sprites.text_spritesheet.width,
                           buff_height - 2 * sprites.text_spritesheet.height - 12,
                           sprites.text_spritesheet, "1234506789"
        );

        buffer.append_text(4, 7, sprites.text_spritesheet, std::to_string(game.getPlayer().lives));

        //buffer.append_object(game.getPlayer());
        buffer.drawObject(game.getPlayer());

        for (auto &alien : game.getAliens()) {
            //buffer.append_object(alien, colors::ORANGE);
            buffer.drawObject(alien, colors::ORANGE);
        }

        for (auto &bullet : game.getAlienBullets()) {
            //buffer.append_object(bullet, colors::RED);
            buffer.drawObject(bullet, colors::RED);
        }

        for (auto &bullet : game.getPlayerBullets()) {
            buffer.drawObject(bullet, colors::RED);
        }

        buffer.append_horizontal_line(16);

        buffer.draw();
        buffer.clear();

        glfwPollEvents();

    }

    // CLEANUP, yes or no?? nono!
    //glfwSetWindowUserPointer(buffer.get_glfw_window(), nullptr);


    /////printf("X: %zu, Y: %zu\n", game.player.x, game.player.y);
    /////printf("Game width: %zu, height %zu\n", game.width, game.height);

    //std::cin.get();
    //s_Finished = true;
    //worker_thread.join();

    #ifdef DEBUG
    io::print_to_stdout("Clean exit, Ran for:");
    #endif
    return EXIT_SUCCESS;
}
