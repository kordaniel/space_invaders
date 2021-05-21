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

void initialize(void) {
    srand(time(nullptr));
}

int main(void)
{
#ifndef NDEBUG
    Timer timer("Total runtime: ");
#endif

    initialize();
    const int32_t buff_width = 224, buff_height = 256;

    Buffer buffer(buff_width, buff_height);
    Game game(buff_width, buff_height);

    glfwSetWindowUserPointer(buffer.get_glfw_window(), &game);

    // Ugly hack to limit the speed of the game in DEBUG mode for now..
    // **************
    uint64_t microsecsInSec = 1 * 1000 * 1000;
    uint64_t updatesPerSecHack = 20;
    using std::chrono::time_point;
    using std::chrono::steady_clock;

    time_point<steady_clock> time_prev_update = steady_clock::now();
    time_point<steady_clock> time_now = steady_clock::now();
    //size_t count = 0;

    while (!glfwWindowShouldClose(buffer.get_glfw_window())) {
        time_now = steady_clock::now();
        if ((time_now - time_prev_update) > std::chrono::microseconds(microsecsInSec / updatesPerSecHack)) {
            game.updateGame();
            time_prev_update = time_now;
        }
        // End ugly hack!
        // **************

        /*
        if (++count % 180 == 0) {
            //timer.reset();
            io::print_to_stdout_varargs("TIME: ", timer.elapsed<std::chrono::nanoseconds>(), "ns");
            io::print_to_stdout_varargs("TIME: ", timer.elapsed<std::chrono::seconds>(), "s");
            io::print_to_stdout_varargs("TIME: ", timer.elapsed<std::chrono::milliseconds>(), "ms");
            io::print_to_stdout_varargs("TIME: ", timer.elapsed<std::chrono::microseconds>(), "us");
            io::print_to_stdout("");
        }
        */
        buffer.append_text(164, 7, Sprites::GetInstance().text_spritesheet, "CREDIT 00");

        buffer.append_text(4, buff_height - Sprites::GetInstance().text_spritesheet.GetHeight() - 7, Sprites::GetInstance().text_spritesheet, "SCORE");
        // Actual score
        buffer.append_text(4 + 2 * Sprites::GetInstance().text_spritesheet.GetWidth(),
                           buff_height - 2 * Sprites::GetInstance().text_spritesheet.GetHeight() - 12,
                           Sprites::GetInstance().text_spritesheet, "1234506789"
        );

        buffer.append_text(4, 7, Sprites::GetInstance().text_spritesheet, std::to_string(game.getPlayer().m_lives));
        int32_t xpos = 13;
        for (int32_t i = 1; i < game.getPlayer().m_lives; ++i) {
            buffer.drawSprite(xpos, 7, Sprites::GetInstance().player_sprite, colors::ORANGE);
            xpos += Sprites::GetInstance().player_sprite.GetWidth() + 3;
        }
#ifndef NDEBUG
        buffer.append_integer(220, 220, Sprites::GetInstance().text_spritesheet, game.getAlienBullets().size(), colors::ORANGE);
#endif
        buffer.drawObject(game.getPlayer());

        for (auto &alien : game.getAliens()) {
            buffer.drawObject(alien, colors::ORANGE);
        }

        for (auto &bullet : game.getAlienBullets()) {
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

#ifndef NDEBUG
    io::print_to_stdout("Clean exit!");
#endif

    return EXIT_SUCCESS;
}
