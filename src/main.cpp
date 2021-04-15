#include "main.h"

int main(void)
{
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

    while (!glfwWindowShouldClose(buffer.get_glfw_window())) {
        game.update_player();
        game.update_bullets();

        buffer.append_object(game.player);
        for (auto &alien : game.aliens) {
            if (alien.lives == 0) continue;
            buffer.append_object(alien);
        }

        for (auto &bullet : game.bullets) {
            buffer.append_object(bullet, colors::RED);
        }

        buffer.append_horizontal_line(16);
        buffer.append_text(4, buff_height - sprites.text_spritesheet.height - 7, sprites.text_spritesheet, "SCORE");
        buffer.append_text(164, 7, sprites.text_spritesheet, "CREDIT 00");

        buffer.append_text(buff_width - (sprites.text_spritesheet.width*2)-1, buff_height - sprites.text_spritesheet.height, sprites.text_spritesheet, "XX");

        // Actual score
        buffer.append_text(4 + 2 * sprites.text_spritesheet.width,
                           buff_height - 2 * sprites.text_spritesheet.height - 12,
                           sprites.text_spritesheet, "40"
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
    return EXIT_SUCCESS;
}
