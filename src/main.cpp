#include "main.h"
/*#include <thread>

static bool s_Finished = false;

void do_heavyCalculation()
{
    while (!s_Finished) {
        io::print_to_stdout("Huhhuh, so heavy lifting going on");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
*/
int main(void)
{
    //std::thread worker_thread(do_heavyCalculation);

    const int32_t buff_width = 224, buff_height = 256;
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
    size_t counter = 0;
    while (!glfwWindowShouldClose(buffer.get_glfw_window())) {
        if (counter == 20) {
            counter = 0;
            game.update_player();
            game.update_aliens();
            game.update_bullets();
        }
        ++counter;

        buffer.append_object(game.player);
        for (auto &alien : game.aliens) {
            if (alien.lives == 0) continue;
            buffer.append_object(alien, colors::ORANGE);
        }

        for (auto &bullet : game.bullets) {
            buffer.append_object(bullet, colors::RED);
        }
        buffer.append_integer(1, 20, sprites.text_spritesheet, 100001, colors::Colors::RED);
        buffer.append_integer(1, 30, sprites.text_spritesheet, 100000, colors::Colors::RED);
        buffer.append_integer(1, 40, sprites.text_spritesheet, 99999, colors::Colors::RED);
        buffer.append_integer(1, 50, sprites.text_spritesheet, 10001, colors::Colors::RED);
        buffer.append_integer(1, 60, sprites.text_spritesheet, 10000, colors::Colors::RED);
        buffer.append_integer(1, 70, sprites.text_spritesheet, 9999, colors::Colors::RED);
        buffer.append_integer(buff_width - 10, 80, sprites.text_spritesheet, 1001, colors::Colors::RED);
        buffer.append_integer(1, 90, sprites.text_spritesheet, 1000, colors::Colors::RED);
        buffer.append_integer(1, 100, sprites.text_spritesheet, 999, colors::Colors::RED);
        buffer.append_integer(1, 110, sprites.text_spritesheet, 1234567890, colors::Colors::RED);
        buffer.append_integer(1, 120, sprites.text_spritesheet, 0, colors::Colors::RED);
        buffer.append_integer(1, 130, sprites.text_spritesheet, 01, colors::Colors::RED);
        

        buffer.append_horizontal_line(16);
        buffer.append_text(4, buff_height - sprites.text_spritesheet.height - 7, sprites.text_spritesheet, "SCORE");
        buffer.append_text(164, 7, sprites.text_spritesheet, "CREDIT 00");

        buffer.append_text(buff_width - (sprites.text_spritesheet.width*2)-1, buff_height - sprites.text_spritesheet.height, sprites.text_spritesheet, "XX");

        //buffer.append_text(0, 0, sprites.text_spritesheet, "XYX");

        // Actual score
        buffer.append_text(4 + 2 * sprites.text_spritesheet.width,
                           buff_height - 2 * sprites.text_spritesheet.height - 12,
                           sprites.text_spritesheet, "1234506789"
        );
        // Actual credits
        buffer.append_text(4, 7, sprites.text_spritesheet, std::to_string(game.player.lives));


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

    return EXIT_SUCCESS;
}
