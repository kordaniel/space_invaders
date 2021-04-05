#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GL_ERROR_CASE(glerror)\
    case glerror: snprintf(error, sizeof(error), "%s", #glerror)

inline void gl_debug(const char* file, int line)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        char error[128];

        switch (err) {
            GL_ERROR_CASE(GL_INVALID_ENUM); break;
            GL_ERROR_CASE(GL_INVALID_VALUE); break;
            GL_ERROR_CASE(GL_INVALID_OPERATION); break;
            GL_ERROR_CASE(GL_INVALID_FRAMEBUFFER_OPERATION); break;
            GL_ERROR_CASE(GL_OUT_OF_MEMORY); break;
            default: snprintf(error, sizeof(error), "%s", "UNKNOWN_ERROR"); break;
        }

        fprintf(stderr, "%s - %s: %d\n", error, file, line);
    }
}

#undef GL_ERROR_CASE


void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key) {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS)
                //glfwSetWindowShouldClose(window, true);
                printf("ESC\n");
            break;
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS)
                //move_dir += 1;
                printf("right_down\n");
            else if (action == GLFW_RELEASE)
                printf("right_up\n");
                //move_dir -= 1;
            break;
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS)
                printf("left_down\n");
                //move_dir -= 1;
            else if (action == GLFW_RELEASE)
                printf("right_up\n");
                //move_dir += 1;
            break;
        case GLFW_KEY_SPACE:
            if (action == GLFW_RELEASE)
                printf("space_up\n");
                //fire_pressed = true;
            break;
        default:
            break;
    }
}


class Shape {
    public:
        size_t width, height;
};

class Sprite: public Shape {
    public:
        uint8_t* data;
        /*
        Sprite() {};
        Sprite(size_t width, size_t height, uint8_t* data)
        {
            this->width  = width;
            this->height = height;
            this->data   = data;
        }*/
        ~Sprite() {
            delete[] data;
        }
};

class Sprites {
    public:
        static const size_t nALIENS = 3;

        Sprite alien_sprites[2 * nALIENS]; /* Two sprites per alien */
        Sprite alien_death_sprite;
        Sprite player_sprite;
        Sprite player_bullet_sprite;
        Sprite alien_bullet_sprites[2];

        Sprite text_spritesheet;
        Sprite number_spritesheet;

        Sprites(void) {
            this->init_aliens();
            this->init_alien_death();
            this->init_player();
            this->init_player_bullet();
            this->init_alien_bullet();
            this->init_spritesheets();
        }

    private:
        void init_aliens(void)
        {
            this->alien_sprites[0].width  = 8;
            this->alien_sprites[0].height = 8;
            this->alien_sprites[0].data   = new uint8_t[8 * 8] {
                0,0,0,1,1,0,0,0, // ...@@...
                0,0,1,1,1,1,0,0, // ..@@@@..
                0,1,1,1,1,1,1,0, // .@@@@@@.
                1,1,0,1,1,0,1,1, // @@.@@.@@
                1,1,1,1,1,1,1,1, // @@@@@@@@
                0,1,0,1,1,0,1,0, // .@.@@.@.
                1,0,0,0,0,0,0,1, // @......@
                0,1,0,0,0,0,1,0  // .@....@.
            };

            this->alien_sprites[1].width  = 8;
            this->alien_sprites[1].height = 8;
            this->alien_sprites[1].data   = new uint8_t[8 * 8] {
                0,0,0,1,1,0,0,0, // ...@@...
                0,0,1,1,1,1,0,0, // ..@@@@..
                0,1,1,1,1,1,1,0, // .@@@@@@.
                1,1,0,1,1,0,1,1, // @@.@@.@@
                1,1,1,1,1,1,1,1, // @@@@@@@@
                0,0,1,0,0,1,0,0, // ..@..@..
                0,1,0,1,1,0,1,0, // .@.@@.@.
                1,0,1,0,0,1,0,1  // @.@..@.@
            };

            this->alien_sprites[2].width  = 11;
            this->alien_sprites[2].height = 8;
            this->alien_sprites[2].data   = new uint8_t[11 * 8] {
                0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
                0,0,0,1,0,0,0,1,0,0,0, // ...@...@...
                0,0,1,1,1,1,1,1,1,0,0, // ..@@@@@@@..
                0,1,1,0,1,1,1,0,1,1,0, // .@@.@@@.@@.
                1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
                1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
                1,0,1,0,0,0,0,0,1,0,1, // @.@.....@.@
                0,0,0,1,1,0,1,1,0,0,0  // ...@@.@@...
            };

            this->alien_sprites[3].width  = 11;
            this->alien_sprites[3].height = 8;
            this->alien_sprites[3].data   = new uint8_t[11 * 8] {
                0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
                1,0,0,1,0,0,0,1,0,0,1, // @..@...@..@
                1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
                1,1,1,0,1,1,1,0,1,1,1, // @@@.@@@.@@@
                1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
                0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
                0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
                0,1,0,0,0,0,0,0,0,1,0  // .@.......@.
            };

            this->alien_sprites[4].width  = 12;
            this->alien_sprites[4].height = 8;
            this->alien_sprites[4].data   = new uint8_t[12 * 8] {
                0,0,0,0,1,1,1,1,0,0,0,0, // ....@@@@....
                0,1,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@@.
                1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
                1,1,1,0,0,1,1,0,0,1,1,1, // @@@..@@..@@@
                1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
                0,0,0,1,1,0,0,1,1,0,0,0, // ...@@..@@...
                0,0,1,1,0,1,1,0,1,1,0,0, // ..@@.@@.@@..
                1,1,0,0,0,0,0,0,0,0,1,1  // @@........@@
            };

            this->alien_sprites[5].width  = 12;
            this->alien_sprites[5].height = 8;
            this->alien_sprites[5].data   = new uint8_t[12 * 8] {
                0,0,0,0,1,1,1,1,0,0,0,0, // ....@@@@....
                0,1,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@@.
                1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
                1,1,1,0,0,1,1,0,0,1,1,1, // @@@..@@..@@@
                1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
                0,0,1,1,1,0,0,1,1,1,0,0, // ..@@@..@@@..
                0,1,1,0,0,1,1,0,0,1,1,0, // .@@..@@..@@.
                0,0,1,1,0,0,0,0,1,1,0,0  // ..@@....@@..
            };
        }

        void init_alien_death(void)
        {
            this->alien_death_sprite.width  = 13;
            this->alien_death_sprite.height = 7;
            this->alien_death_sprite.data   = new uint8_t[13 * 7] {
                0,1,0,0,1,0,0,0,1,0,0,1,0, // .@..@...@..@.
                0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
                0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
                1,1,0,0,0,0,0,0,0,0,0,1,1, // @@.........@@
                0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
                0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
                0,1,0,0,1,0,0,0,1,0,0,1,0  // .@..@...@..@.
            };
        }

        void init_player(void)
        {
            this->player_sprite.width  = 11;
            this->player_sprite.height = 7;
            this->player_sprite.data   = new uint8_t[11 * 7] {
                0,0,0,0,0,1,0,0,0,0,0, // .....@.....
                0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
                0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
                0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
                1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
                1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
                1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
            };
        }

        void init_player_bullet(void)
        {
            this->player_bullet_sprite.width  = 1;
            this->player_bullet_sprite.height = 3;
            this->player_bullet_sprite.data   = new uint8_t[1 * 3] {
                1, // @
                1, // @
                1  // @
            };
        }

        void init_alien_bullet(void)
        {
            this->alien_bullet_sprites[0].width  = 3;
            this->alien_bullet_sprites[0].height = 7;
            this->alien_bullet_sprites[0].data   = new uint8_t[3 * 7] {
                0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0
            };

            this->alien_bullet_sprites[1].width  = 3;
            this->alien_bullet_sprites[1].height = 7;
            this->alien_bullet_sprites[1].data   = new uint8_t[3 * 7] {
                0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0
            };
        }

        void init_spritesheets(void)
        {
            /* TODO: Refactor to use same ASCII spritesheet for all symbols.. */
            this->text_spritesheet.width  = 5;
            this->text_spritesheet.height = 7;
            this->text_spritesheet.data   = new uint8_t[65 * 5 * 7] {
                /* ASCII characters in the range [32,96] == ['space','`'] */
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,
                0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                0,1,0,1,0,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,0,1,0,1,0,
                0,0,1,0,0,0,1,1,1,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,0,1,0,0,
                1,1,0,1,0,1,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,1,0,1,0,1,1,
                0,1,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,0,1,0,0,0,1,0,1,1,1,1,
                0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,
                1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,
                0,0,1,0,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,1,0,0,
                0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
                0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,

                0,1,1,1,0,1,0,0,0,1,1,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,0,1,1,1,0,
                0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,0,
                0,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,
                1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
                0,0,0,1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,1,0,
                1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
                0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
                1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,
                0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
                0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,

                0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,
                0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,
                0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
                1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,
                0,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,
                0,1,1,1,0,1,0,0,0,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,0,1,0,0,0,1,0,1,1,1,0,

                0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,
                1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,
                0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,1,1,0,
                1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,
                1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,
                1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,
                0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,0,1,1,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
                1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,
                0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,0,
                0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
                1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,1,0,0,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,1,
                1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,
                1,0,0,0,1,1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,
                1,0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1,
                0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
                1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,
                0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,0,1,1,0,1,1,1,1,
                1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,1,
                0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,0,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,0,
                1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,
                1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
                1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,
                1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,0,0,0,1,
                1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,
                1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,
                1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,

                0,0,0,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,
                0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,
                1,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,0,
                0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
                0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            };

            /* Copy of Sprite object, both objects contain an pointer to the same
             * .data uint8_t[] spritesheet array.
             * Move the pointer to point to the ascii val of '0' in the
             * number_spritesheet object.
             */
            this->number_spritesheet = this->text_spritesheet;
            this->number_spritesheet.data += 16 * (5 * 7);
        }
};


class Buffer: public Shape
{
    public:
        Buffer(size_t width, size_t height)
        {
            this->width  = width;
            this->height = height;

            data = new uint32_t[width * height];
            clear(0);

            initialize_glfw_window();
            initialize_opengl();
            initialize_buffer_texture();
            initialize_shaders();
        }

        ~Buffer(void) {
            delete[] data;
        }

        void clear(uint32_t color)
        {
            for (size_t i = 0; i < width * height; ++i) {
                data[i] = color;
            }
        }

        void draw(void)
        {
            glTexSubImage2D(
                GL_TEXTURE_2D, 0, 0, 0,
                width, height,
                GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
                data
            );
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glfwSwapBuffers(glfw_window);
        }

        GLFWwindow* get_glfw_window(void)
        {
            return this->glfw_window;
        }


    private:
        uint32_t* data; /* 8 bits each for R,G,B,alpha value */
        GLFWwindow *glfw_window;
        GLenum err;
        GLuint buffer_texture;
        GLuint fullscreen_triangle_vao;
        GLuint shader_id;
        GLint location;

        void initialize_glfw_window(void)
        {
            glfwSetErrorCallback(::error_callback);
            if (!glfwInit()) {
                fprintf(stderr, "[ERROR]: when trying to initialize glfw.\n");
                exit(EXIT_FAILURE);
            }

            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

            this->glfw_window = glfwCreateWindow(width, height, "Space Invaders", NULL, NULL);
            if (!glfw_window) {
                fprintf(stderr, "[ERROR]: when trying to create glfw window.\n");
                glfwTerminate();
                exit(EXIT_FAILURE);
            }

            glfwSetKeyCallback(glfw_window, ::key_callback);
            glfwMakeContextCurrent(glfw_window);
            err = glewInit();
            if (err != GLEW_OK) {
                fprintf(stderr, "[ERROR]: initializing GLEW.\n");
                fprintf(stderr, "%s\n", glewGetErrorString(err));
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
        }

        void initialize_opengl(void)
        {
            int glVersion[2] = {-1, 1};
            glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
            glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

            gl_debug(__FILE__, __LINE__);

            printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);
            printf("Renderer used: %s\n", glGetString(GL_RENDERER));
            printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

            glfwSwapInterval(1); /* vsync ON */
            glClearColor(1.0, 0.0, 0.0, 1.0);
        }

        void initialize_buffer_texture(void)
        {
            glGenTextures(1, &buffer_texture);
            glBindTexture(GL_TEXTURE_2D, buffer_texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0,
                        GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        void initialize_shaders(void)
        {
            glGenVertexArrays(1, &fullscreen_triangle_vao);
            static const char* fragment_shader =
                "\n"
                "#version 330\n"
                "\n"
                "uniform sampler2D buffer;\n"
                "noperspective in vec2 TexCoord;\n"
                "\n"
                "out vec3 outColor;\n"
                "\n"
                "void main(void) {\n"
                "   outColor = texture(buffer, TexCoord).rgb;\n"
                "}\n";

            static const char* vertex_shader =
                "\n"
                "#version 330\n"
                "\n"
                "noperspective out vec2 TexCoord;\n"
                "\n"
                "void main(void){\n"
                "\n"
                "    TexCoord.x = (gl_VertexID == 2)? 2.0: 0.0;\n"
                "    TexCoord.y = (gl_VertexID == 1)? 2.0: 0.0;\n"
                "    \n"
                "    gl_Position = vec4(2.0 * TexCoord - 1.0, 0.0, 1.0);\n"
                "}\n";

            shader_id = glCreateProgram();

            init_shader(vertex_shader, GL_VERTEX_SHADER);
            init_shader(fragment_shader, GL_FRAGMENT_SHADER);

            glLinkProgram(shader_id);

            if (!validate_program(shader_id)) {
                fprintf(stderr, "[ERROR] validating shader.\n");
                glfwTerminate();
                glDeleteVertexArrays(1, &fullscreen_triangle_vao);
                delete[] data;
                exit(EXIT_FAILURE);
            }

            glUseProgram(shader_id);

            location = glGetUniformLocation(shader_id, "buffer");
            glUniform1i(location, 0);

            glDisable(GL_DEPTH_TEST);
            glActiveTexture(GL_TEXTURE0);

            glBindVertexArray(fullscreen_triangle_vao);
        }

        void init_shader(const char *shader_code, GLenum type)
        {
            GLuint shader = glCreateShader(type);

            glShaderSource(shader, 1, &shader_code, 0);
            glCompileShader(shader);
            validate_shader(shader, shader_code);
            glAttachShader(shader_id, shader);

            glDeleteShader(shader);
        }

        void validate_shader(GLuint shader, const char *file = 0)
        {
            static const unsigned int BUFFER_SIZE = 512;
            char buffer[BUFFER_SIZE];
            GLsizei length = 0;

            glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
            if (length > 0) {
                fprintf(stderr, "[ERROR]: Shader %d(%s) compile error: %s\n",
                        shader, (file? file: ""), buffer);
            }
        }

        bool validate_program(GLuint program)
        {
            static const GLsizei BUFFER_SIZE = 512;
            GLchar buffer[BUFFER_SIZE];
            GLsizei length = 0;

            glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
            if (length > 0) {
                fprintf(stderr, "[ERROR]: Program %d link error: %s\n", program, buffer);
                return false;
            }

            return true;
        }
};



int main(void)
{
    const size_t buff_width = 224, buff_height = 256;
    Sprites sprites;
    Buffer buffer(buff_width, buff_height);
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
        buffer.clear((uint32_t) (r << 24)| (g << 16) | (b << 8) | a);

        buffer.draw();
        glfwPollEvents();

    }
    return EXIT_SUCCESS;
}
