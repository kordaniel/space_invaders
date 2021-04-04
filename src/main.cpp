#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GAME_MAX_BULLETS 128

int move_dir = 0;
bool fire_pressed = 0;


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

void validate_shader(GLuint shader, const char *file = 0)
{
    static const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
    if (length > 0) {
        printf("Shader %d(%s) compile error: %s\n", shader, (file? file: ""), buffer);
    }
}

bool validate_program(GLuint program)
{
    static const GLsizei BUFFER_SIZE = 512;
    GLchar buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
    if (length > 0) {
        printf("Program %d link error: %s\n", program, buffer);
        return false;
    }

    return true;
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key) {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS)
                move_dir += 1;
            else if (action == GLFW_RELEASE)
                move_dir -= 1;
            break;
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS)
                move_dir -= 1;
            else if (action == GLFW_RELEASE)
                move_dir += 1;
            break;
        case GLFW_KEY_SPACE:
            if (action == GLFW_RELEASE)
                fire_pressed = true;
            break;
        default:
            break;
    }
}

enum AlienType: uint8_t {
    ALIEN_DEAD   = 0,
    ALIEN_TYPE_A = 1,
    ALIEN_TYPE_B = 2,
    ALIEN_TYPE_C = 3
};

/*
 * Colors are 32bits, 8 bits each for R,G,B and alpha values.
 */
enum Colors: uint32_t {
    RED      = (uint32_t) (128 << 24) |                          255,
    ORANGE   = (uint32_t) (160 << 24) | (96 << 16)             | 255,
    BG_GREEN = (uint32_t) (50  << 24) | (80 << 16) | (75 << 8) | 255
};

uint32_t xorshift32(uint32_t* rng)
{
    uint32_t x = *rng;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *rng = x;
    return x;
}

double random(uint32_t* rng)
{
    return (double) xorshift32(rng) / std::numeric_limits<uint32_t>::max();
}

uint32_t rgb_to_uint32(uint8_t r, uint8_t g, uint8_t b)
{
    return (r << 24) | (g << 16) | (b << 8) | 255;
}

class Shape {
    public:
        size_t width, height;
};

class CartesianPoint {
    public:
        size_t x, y;
};

class Sprite: public Shape {
    public:
        uint8_t* data;
};

class SpriteAnimation {
    public:
        bool loop;
        size_t num_frames;
        size_t frame_duration;
        size_t time;
        Sprite** frames;
};

class Bullet: public CartesianPoint {
    public:
        int dir;
};

class Player: public CartesianPoint {
    public:
        size_t life;
        int move_dir;
};

class Alien: public CartesianPoint {
    public:
        uint8_t type;
};

class Game: public Shape {
    public:
        size_t num_aliens;
        size_t num_bullets;

        size_t alien_swarm_position;
        size_t alien_swarm_max_position;
        size_t aliens_killed;
        //size_t alien_update_timer;
        size_t alien_update_frequency;

        bool should_change_speed;
        uint8_t* death_counters;

        Alien* aliens;
        Sprite* alien_sprites;
        Sprite alien_death_sprite;
        Sprite player_sprite;

        SpriteAnimation* alien_animation;

        Player player;
        Bullet bullets[GAME_MAX_BULLETS];

        Game(size_t width, size_t height,
             Sprite* alien_sprites,
             SpriteAnimation* alien_animation,
             Sprite alien_death_sprite,
             Sprite player_sprite,
             size_t num_aliens, 
             size_t alien_update_frequency,
             size_t num_bullets = 0)
        {
            printf("Initializing gameobject\n");
            this->width = width;
            this->height = height;

            this->num_aliens = num_aliens;
            this->num_bullets = num_bullets;
            this->alien_swarm_position = 24;
            this->alien_swarm_max_position = width - 16 * 11 -3;
            this->aliens_killed = 0;
            //this->alien_update_timer = 0;
            this->alien_update_frequency = alien_update_frequency;
            this->should_change_speed = false;

            this->alien_sprites = alien_sprites;
            this->alien_animation = alien_animation;
            this->alien_death_sprite = alien_death_sprite;
            this->player_sprite = player_sprite;

            this->player.x = this->width/2 - 5;
            this->player.y = 32;
            this->player.life = 3;
            this->player.move_dir = 0;

            this->initialize_aliens_swarm();
            this->initialize_alien_death_counters();
        }

        ~Game(void) {
            printf("Destructing gameobject\n");
            delete[] this->aliens;
            delete[] this->death_counters;
        }

        void update_status() {
            //this->simulate_aliens();
            this->update_alien_death_counters();
            this->update_player_position();
        }

    private:
        void initialize_aliens_swarm(void) {
            this->aliens = new Alien[this->num_aliens];
            for (size_t xi = 0; xi < 11; ++xi) {
                for (size_t yi = 0; yi < 5; ++yi) {
                    Alien& alien = this->aliens[xi * 5 + yi];
                    alien.type = (5 - yi) / 2 + 1;
                    const Sprite& sprite = this->alien_sprites[2 * (alien.type - 1)];
                    alien.x = 16 * xi + alien_swarm_position + (this->alien_death_sprite.width - sprite.width)/2;
                    alien.y = 17 * yi + 128;
                }
            }
        }

        void initialize_alien_death_counters(void) {
            this->death_counters = new uint8_t[this->num_aliens];
            for (size_t i = 0; i < this->num_aliens; ++i) {
                this->death_counters[i] = 10;
            }
        }

        void update_alien_death_counters(void) {
            for (size_t i = 0; i < this->num_aliens; ++i) {
                if (this->aliens[i].type == ALIEN_DEAD && this->death_counters[i])
                    --this->death_counters[i];
            }
        }

        void update_player_position() {
            Player& p = this->player;
            p.move_dir = 2*move_dir;

            if (!p.move_dir) {
                return;
            }

            if (p.x + p.move_dir + this->player_sprite.width >= this->width) {
                /* Make sure we stay within bounds to the right */
                p.x = this->width - this->player_sprite.width;
            } else {
                /* Make sure we stay within bounds to the left */
                p.x = ((int) p.x + p.move_dir) < 0
                        ? 0
                        : p.x + p.move_dir;
            }
        }
};

class Buffer: public Shape {
    public:
        uint32_t* data;
        Sprite text_spritesheet;
        Sprite number_spritesheet;
        Sprite player_sprite;
        Sprite player_bullet_sprite;
        GLFWwindow* window;
        Sprite alien_death_sprite;
        SpriteAnimation* alien_animation;
        SpriteAnimation alien_bullet_animation;
        Sprite* alien_bullet_sprite;

        Buffer(const size_t buff_width,
               const size_t buff_height,
               GLFWwindow* window)
        {
            this->width = buff_width;
            this->height = buff_height;
            this->window = window;
            this->data = new uint32_t[width * height];
        }

        ~Buffer(void) {
            delete[] data;
        }

        void clear(uint32_t color) {
            for (size_t i = 0; i < width * height; ++i)
                data[i] = color;
        }

        void draw_sprite(const Sprite& sprite, size_t x, size_t y, uint32_t color) {
            for (size_t xi = 0; xi < sprite.width; ++xi) {
                for (size_t yi = 0; yi < sprite.height; ++yi) {
                    if (sprite.data[yi * sprite.width + xi] && (x + xi) < this->width) {
                        this->data[(sprite.height - 1 + y - yi) * this->width + (x + xi)] = color;
                    }
                }
            }
        }

        void draw_number(size_t number, size_t xpos, size_t ypos, uint32_t color) {
            uint8_t digits[64];
            size_t num_digits = 0;
            size_t current_num = number;
            do {
                digits[num_digits++] = current_num % 10;
                current_num /= 10;
            } while (current_num > 0);

            size_t xp = xpos;
            size_t stride = this->number_spritesheet.width * this->number_spritesheet.height;
            Sprite sprite = this->number_spritesheet;
            for (size_t i = 0; i < num_digits; ++i) {
                uint8_t digit = digits[num_digits - i - 1];
                sprite.data = this->number_spritesheet.data + digit * stride;
                this->draw_sprite(sprite, xp, ypos, color);
                xp += sprite.width + 1;
            }
        }

        void draw_text(const char* text, size_t xpos, size_t ypos, uint32_t color) {
            size_t xp = xpos;
            size_t stride = this->text_spritesheet.width * this->text_spritesheet.height;
            Sprite sprite = this->text_spritesheet;
            for (const char* charptr = text; *charptr != '\0'; ++charptr) {
                char character = *charptr - 32;
                if (character < 0 || character >= 65) {
                    continue;
                }
                sprite.data = this->text_spritesheet.data + character * stride;
                this->draw_sprite(sprite, xp, ypos, color);
                xp += sprite.width + 1;
            }
        }

        void draw_game(const Game& game, size_t score, size_t credits) {
            this->draw_text(
                "SCORE",
                4,
                game.height - this->text_spritesheet.height - 7,
                rgb_to_uint32(160, 96, 0)
            );
            this->draw_number(
                score,
                4 + 2 * this->number_spritesheet.width,
                game.height - 2 * this->number_spritesheet.height - 12,
                rgb_to_uint32(160, 96, 0)
            );

            this->draw_number(game.player.life, 4, 7, rgb_to_uint32(160, 96, 0));
            size_t xp = 11 + this->number_spritesheet.width;
            for (size_t i = 1; i < game.player.life; ++i) {
                /* NOTE: We draw only the "remaining lifes" */
                this->draw_sprite(this->player_sprite, xp, 7, rgb_to_uint32(160, 96, 0));
                xp += this->player_sprite.width + 2;
            }

            {
                for (size_t i = 0; i < game.width; ++i) {
                    /* NOTE: Draw vertical line to separate status section from gamefield */
                    this->data[game.width * 16 + i] = rgb_to_uint32(160, 96, 0);;
                }
                char credit_text[16];
                sprintf(credit_text, "CREDIT %02lu", credits);
                this->draw_text(credit_text, 164, 7, rgb_to_uint32(160, 96, 0));
            }

            this->draw_sprite(this->player_sprite, game.player.x, game.player.y, rgb_to_uint32(160, 96, 0));

            for (size_t ai = 0; ai < game.num_aliens; ++ai) {
                if (!game.death_counters[ai]) {
                    continue;
                }

                const Alien& alien = game.aliens[ai];
                if (alien.type == ALIEN_DEAD) {
                    this->draw_sprite(this->alien_death_sprite, alien.x, alien.y, ORANGE);
                    continue;
                }

                const SpriteAnimation& animation = this->alien_animation[alien.type - 1];
                size_t current_frame = animation.time / animation.frame_duration;
                const Sprite& sprite = *animation.frames[current_frame];
                this->draw_sprite(sprite, alien.x, alien.y, ORANGE);
            }

            for (size_t bi = 0; bi < game.num_bullets; ++bi) {
                const Bullet& bullet = game.bullets[bi];
                const Sprite* sprite;
                if (bullet.dir > 0) {
                    sprite = &this->player_bullet_sprite;
                } else {
                    size_t cf = this->alien_bullet_animation.time / this->alien_bullet_animation.frame_duration;
                    sprite = &this->alien_bullet_sprite[cf];
                }
                this->draw_sprite(*sprite, bullet.x, bullet.y, ORANGE);
            }
            this->draw();
        }

        void draw_gameover(const Game& game, size_t score) {
            this->draw_text(
                "GAME OVER",
                game.width / 2 - 30, game.height / 2,
                rgb_to_uint32(128, 0, 0)
            );
            this->draw_text(
                "SCORE",
                4,
                game.height - this->text_spritesheet.height - 7,
                rgb_to_uint32(128, 0, 0)
            );
            this->draw_number(
                score,
                4 + 2 * this->number_spritesheet.width,
                game.height - 2 * this->number_spritesheet.height - 12,
                rgb_to_uint32(128, 0, 0)
            );
            this->draw();
        }

    private:
        void draw() {
            glTexSubImage2D(
                GL_TEXTURE_2D, 0, 0, 0,
                this->width, this->height,
                GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
                this->data
            );
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glfwSwapBuffers(this->window);
        }
};

/**
 * NOTE: We don't actually check if the pixels collide,
 *       only if the sprite rectangles do.
 */
bool sprite_overlap_check(
        const Sprite& sp_a, size_t x_a, size_t y_a,
        const Sprite& sp_b, size_t x_b, size_t y_b)
{
    if (x_a < x_b + sp_b.width && x_a + sp_a.width > x_b &&
        y_a < y_b + sp_b.height && y_a + sp_a.height > y_b) {
        return true;
    }

    return false;
}

GLFWwindow* initialize(const size_t buffer_width, const size_t buffer_height)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        return NULL;
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window; /* Windowed window & OpenGL context */
    window = glfwCreateWindow(buffer_width, buffer_height, "Space Invaders", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Error creating GLFW window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    
    /* This might be needed to be set on GLEW versions 1.13 or earlier
    *  glewExperimental = GL_TRUE;
    */

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error initializing GLEW.\n");
        fprintf(stderr, "%s\n", glewGetErrorString(err));
        glfwTerminate();
        return NULL;
    }

    return window;
}

void initialize_opengl()
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

Buffer initialize_buffer(const size_t buffer_width, const size_t buffer_height,
                         GLFWwindow* window)
{
    Buffer buffer(buffer_width, buffer_height, window); /* Graphics buffer */
    buffer.clear(0);
    return buffer;
}

int main(void)
{
    const size_t buffer_width = 224;
    const size_t buffer_height = 256;

    GLFWwindow* window = initialize(buffer_width, buffer_height);
    if (!window) {
        return EXIT_FAILURE;
    }
    
    initialize_opengl();
    Buffer buffer = initialize_buffer(buffer_width, buffer_height, window);

    GLuint buffer_texture; /* Texture for presenting buffer to OpenGL */
    glGenTextures(1, &buffer_texture);
    glBindTexture(GL_TEXTURE_2D, buffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, buffer.width, buffer.height, 0,
                 GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLuint fullscreen_triangle_vao; /* vao for fullscreen triangle */
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

    GLuint shader_id = glCreateProgram();

    {
        GLuint shader_vp = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(shader_vp, 1, &vertex_shader, 0);
        glCompileShader(shader_vp);
        validate_shader(shader_vp, vertex_shader);
        glAttachShader(shader_id, shader_vp);

        glDeleteShader(shader_vp);
    }

    {
        GLuint shader_fp = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(shader_fp, 1, &fragment_shader, 0);
        glCompileShader(shader_fp);
        validate_shader(shader_fp, fragment_shader);
        glAttachShader(shader_id, shader_fp);

        glDeleteShader(shader_fp);
    }

    glLinkProgram(shader_id);

    if (!validate_program(shader_id)) {
        fprintf(stderr, "Error while validating shader.\n");
        glfwTerminate();
        glDeleteVertexArrays(1, &fullscreen_triangle_vao);
        delete[] buffer.data;
        return -1;
    }

    glUseProgram(shader_id);

    GLint location = glGetUniformLocation(shader_id, "buffer");
    glUniform1i(location, 0);

    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(fullscreen_triangle_vao);

    Sprite alien_sprites[6];
    //Sprite* alien_sprites = (Sprite*)malloc(sizeof(Sprite) * 6);
    alien_sprites[0].width  = 8;
    alien_sprites[0].height = 8;
    alien_sprites[0].data   = new uint8_t[64]
    {
        0,0,0,1,1,0,0,0, // ...@@...
        0,0,1,1,1,1,0,0, // ..@@@@..
        0,1,1,1,1,1,1,0, // .@@@@@@.
        1,1,0,1,1,0,1,1, // @@.@@.@@
        1,1,1,1,1,1,1,1, // @@@@@@@@
        0,1,0,1,1,0,1,0, // .@.@@.@.
        1,0,0,0,0,0,0,1, // @......@
        0,1,0,0,0,0,1,0  // .@....@.
    };

    alien_sprites[1].width  = 8;
    alien_sprites[1].height = 8;
    alien_sprites[1].data   = new uint8_t[64]
    {
        0,0,0,1,1,0,0,0, // ...@@...
        0,0,1,1,1,1,0,0, // ..@@@@..
        0,1,1,1,1,1,1,0, // .@@@@@@.
        1,1,0,1,1,0,1,1, // @@.@@.@@
        1,1,1,1,1,1,1,1, // @@@@@@@@
        0,0,1,0,0,1,0,0, // ..@..@..
        0,1,0,1,1,0,1,0, // .@.@@.@.
        1,0,1,0,0,1,0,1  // @.@..@.@
    };

    alien_sprites[2].width  = 11;
    alien_sprites[2].height = 8;
    alien_sprites[2].data   = new uint8_t[88]
    {
        0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
        0,0,0,1,0,0,0,1,0,0,0, // ...@...@...
        0,0,1,1,1,1,1,1,1,0,0, // ..@@@@@@@..
        0,1,1,0,1,1,1,0,1,1,0, // .@@.@@@.@@.
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
        1,0,1,0,0,0,0,0,1,0,1, // @.@.....@.@
        0,0,0,1,1,0,1,1,0,0,0  // ...@@.@@...
    };

    alien_sprites[3].width  = 11;
    alien_sprites[3].height = 8;
    alien_sprites[3].data   = new uint8_t[88]
    {
        0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
        1,0,0,1,0,0,0,1,0,0,1, // @..@...@..@
        1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
        1,1,1,0,1,1,1,0,1,1,1, // @@@.@@@.@@@
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
        0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
        0,1,0,0,0,0,0,0,0,1,0  // .@.......@.
    };

    alien_sprites[4].width  = 12;
    alien_sprites[4].height = 8;
    alien_sprites[4].data   = new uint8_t[96]
    {
        0,0,0,0,1,1,1,1,0,0,0,0, // ....@@@@....
        0,1,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@@.
        1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
        1,1,1,0,0,1,1,0,0,1,1,1, // @@@..@@..@@@
        1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
        0,0,0,1,1,0,0,1,1,0,0,0, // ...@@..@@...
        0,0,1,1,0,1,1,0,1,1,0,0, // ..@@.@@.@@..
        1,1,0,0,0,0,0,0,0,0,1,1  // @@........@@
    };

    alien_sprites[5].width  = 12;
    alien_sprites[5].height = 8;
    alien_sprites[5].data   = new uint8_t[96]
    {
        0,0,0,0,1,1,1,1,0,0,0,0, // ....@@@@....
        0,1,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@@.
        1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
        1,1,1,0,0,1,1,0,0,1,1,1, // @@@..@@..@@@
        1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
        0,0,1,1,1,0,0,1,1,1,0,0, // ..@@@..@@@..
        0,1,1,0,0,1,1,0,0,1,1,0, // .@@..@@..@@.
        0,0,1,1,0,0,0,0,1,1,0,0  // ..@@....@@..
    };

    Sprite alien_death_sprite;
    alien_death_sprite.width  = 13;
    alien_death_sprite.height = 7;
    alien_death_sprite.data   = new uint8_t[91]
    {
        0,1,0,0,1,0,0,0,1,0,0,1,0, // .@..@...@..@.
        0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
        0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
        1,1,0,0,0,0,0,0,0,0,0,1,1, // @@.........@@
        0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
        0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
        0,1,0,0,1,0,0,0,1,0,0,1,0  // .@..@...@..@.
    };

    Sprite player_sprite;
    player_sprite.width  = 11;
    player_sprite.height = 7;
    player_sprite.data   = new uint8_t[77]
    {
        0,0,0,0,0,1,0,0,0,0,0, // .....@.....
        0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
        0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
        0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
    };

    Sprite player_bullet_sprite;
    player_bullet_sprite.width = 1;
    player_bullet_sprite.height = 3;
    player_bullet_sprite.data = new uint8_t[3]
    {
        1, // @
        1, // @
        1  // @
    };

    Sprite alien_bullet_sprite[2];
    alien_bullet_sprite[0].width  = 3;
    alien_bullet_sprite[0].height = 7;
    alien_bullet_sprite[0].data   = new uint8_t[21]
    {
        0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0
    };

    alien_bullet_sprite[1].width  = 3;
    alien_bullet_sprite[1].height = 7;
    alien_bullet_sprite[1].data   = new uint8_t[21]
    {
        0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0
    };

    Sprite text_spritesheet;
    text_spritesheet.width  = 5;
    text_spritesheet.height = 7;
    text_spritesheet.data   = new uint8_t[65 * 35]
    {
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

    Sprite number_spritesheet = text_spritesheet;
    number_spritesheet.data += 16 * 35;

    SpriteAnimation alien_animation[3];
    size_t alien_update_frequency = 120;
    for (size_t i = 0; i < 3; ++i) {
        alien_animation[i].loop = true;
        alien_animation[i].num_frames = 2;
        alien_animation[i].frame_duration = alien_update_frequency;
        alien_animation[i].time = 0;

        alien_animation[i].frames = new Sprite*[2];
        alien_animation[i].frames[0] = &alien_sprites[2 * i];
        alien_animation[i].frames[1] = &alien_sprites[2 * i + 1];
    }

    SpriteAnimation alien_bullet_animation;
    alien_bullet_animation.loop = true;
    alien_bullet_animation.num_frames = 2;
    alien_bullet_animation.frame_duration = 5;
    alien_bullet_animation.time = 0;

    alien_bullet_animation.frames = new Sprite*[2];
    alien_bullet_animation.frames[0] = &alien_bullet_sprite[0];
    alien_bullet_animation.frames[1] = &alien_bullet_sprite[1];

    Game game(buffer_width, buffer_height, alien_sprites, alien_animation, alien_death_sprite, player_sprite, 55, 120);

    /* SET MEMBERS->MOVE TO PROPER INITIALIZATION */
    buffer.text_spritesheet = text_spritesheet;
    buffer.number_spritesheet = number_spritesheet;
    buffer.player_sprite = player_sprite;
    buffer.player_bullet_sprite = player_bullet_sprite;
    buffer.alien_animation = alien_animation;
    buffer.alien_death_sprite = alien_death_sprite;
    buffer.alien_bullet_animation = alien_bullet_animation;
    buffer.alien_bullet_sprite = alien_bullet_sprite;

    size_t alien_swarm_position = 24;
    size_t alien_swarm_max_position = game.width - 16 * 11 - 3;
    size_t alien_update_timer = 0;

    uint32_t clear_color = BG_GREEN;//rgb_to_uint32(50, 80, 75);
    uint32_t rng = 13;

    int alien_move_dir = 4;

    size_t score = 0;
    size_t credits = 0;

    while (!glfwWindowShouldClose(window)) {
        buffer.clear(clear_color);

        if (game.player.life == 0) {
            buffer.draw_gameover(game, score);
            glfwPollEvents();
            continue;
        }
        
        
        buffer.draw_game(game, score, credits);



        // Simulate bullets
        for (size_t bi = 0; bi < game.num_bullets; ++bi) {
            game.bullets[bi].y += game.bullets[bi].dir;
            if (game.bullets[bi].y >= game.height || game.bullets[bi].y < player_bullet_sprite.height) {
                game.bullets[bi] = game.bullets[game.num_bullets - 1];
                --game.num_bullets;
                continue;
            }

            if (game.bullets[bi].dir < 0) {
                // Alien bullet
                bool overlap = sprite_overlap_check(
                    alien_bullet_sprite[0], game.bullets[bi].x, game.bullets[bi].y,
                    player_sprite, game.player.x, game.player.y
                );

                if (overlap) {
                    --game.player.life;
                    game.bullets[bi] = game.bullets[game.num_bullets - 1];
                    --game.num_bullets;
                    /* NOTE: The rest of the frame is still going to be simulated.
                     *       perhaps we need to check if the game is over or not. */
                    break;
                }
            } else {
                // Player bullet
                for (size_t bj = 0; bj < game.num_bullets; ++bj) {
                    if (bi == bj) {
                        continue;
                    }

                    bool overlap = sprite_overlap_check(
                        player_bullet_sprite, game.bullets[bi].x, game.bullets[bi].y,
                        alien_bullet_sprite[0], game.bullets[bj].x, game.bullets[bj].y
                    );

                    if (overlap) {
                        // NOTE: Make sure it works.
                        if (bj == game.num_bullets - 1) {
                            game.bullets[bi] = game.bullets[game.num_bullets - 2];
                        } else if (bi == game.num_bullets - 1) {
                            game.bullets[bi] = game.bullets[game.num_bullets - 2];
                        } else {
                            game.bullets[(bi < bj)? bi : bj] = game.bullets[game.num_bullets - 1];
                            game.bullets[(bi < bj)? bj : bi] = game.bullets[game.num_bullets - 2];
                        }
                        game.num_bullets -= 2;
                        break;
                    }
                }

                // Check hit
                for (size_t ai = 0; ai < game.num_aliens; ++ai) {
                    const Alien& alien = game.aliens[ai];
                    if (alien.type == ALIEN_DEAD) {
                        continue;
                    }

                    const SpriteAnimation& animation = alien_animation[alien.type - 1];
                    size_t current_frame = animation.time / animation.frame_duration;
                    const Sprite& alien_sprite = *animation.frames[current_frame];
                    bool overlap = sprite_overlap_check(
                        player_bullet_sprite, game.bullets[bi].x, game.bullets[bi].y,
                        alien_sprite, alien.x, alien.y
                    );

                    if (overlap) {
                        score += 10 * (4 - game.aliens[ai].type);
                        game.aliens[ai].type = ALIEN_DEAD;
                        game.aliens[ai].x -= (alien_death_sprite.width - alien_sprite.width) / 2;
                        game.bullets[bi] = game.bullets[game.num_bullets - 1];
                        --game.num_bullets;
                        ++game.aliens_killed;

                        if (game.aliens_killed % 15 == 0) {
                            game.should_change_speed = true;
                        }

                        break;
                    }
                }
            }
        }

        // Simulate aliens
        if (game.should_change_speed) {
            game.should_change_speed = false;
            alien_update_frequency /= 2;
            for (size_t i = 0; i < 3; ++i) {
                alien_animation[i].frame_duration = alien_update_frequency;
            }
        }

        if (alien_update_timer >= alien_update_frequency) {
            /* UPDATE_DEATH_COUNTERS BEFORE executing this if-statement !! (?) */
            alien_update_timer = 0;
            if ((int) alien_swarm_position + alien_move_dir < 0) {
                alien_move_dir *= -1;
                /* TODO: Perhaps if aliens get close enough to player, we need to check
                 *       for overlap. What happens when aliens move over line y = 0 line? */
                for (size_t ai = 0; ai < game.num_aliens; ++ai) {
                    Alien& alien = game.aliens[ai];
                    alien.y -= 8;
                }
            } else if (alien_swarm_position > alien_swarm_max_position - alien_move_dir) {
                alien_move_dir *= -1;
            }
            alien_swarm_position += alien_move_dir;

            for (size_t ai = 0; ai < game.num_aliens; ++ai) {
                Alien& alien = game.aliens[ai];
                alien.x += alien_move_dir;
            }

            if (game.aliens_killed < game.num_aliens) {
                size_t rai = game.num_aliens * random(&rng);
                while (game.aliens[rai].type == ALIEN_DEAD) {
                    rai = game.num_aliens * random(&rng);
                }
                const Sprite& alien_sprite = *alien_animation[game.aliens[rai].type - 1].frames[0];
                game.bullets[game.num_bullets].x = game.aliens[rai].x + alien_sprite.width / 2;
                game.bullets[game.num_bullets].y = game.aliens[rai].y - alien_bullet_sprite[0].height;
                game.bullets[game.num_bullets].dir = -2;
                ++game.num_bullets;
            }
        }

        // Update animations
        for (size_t i = 0; i < 3; ++i) {
            ++alien_animation[i].time;
            if (alien_animation[i].time >= alien_animation[i].num_frames * alien_animation[i].frame_duration) {
                alien_animation[i].time = 0;
            }
        }
        /* DEV DEV */
        ++buffer.alien_bullet_animation.time;
        if (buffer.alien_bullet_animation.time >= buffer.alien_bullet_animation.num_frames * buffer.alien_bullet_animation.frame_duration) {
            buffer.alien_bullet_animation.time = 0;
        }
        /* DEV DEV END */
        ++alien_bullet_animation.time;
        if (alien_bullet_animation.time >= alien_bullet_animation.num_frames * alien_bullet_animation.frame_duration) {
            alien_bullet_animation.time = 0;
        }

        ++alien_update_timer;

        game.update_status();

        if (game.aliens_killed < game.num_aliens) {
            size_t ai = 0;
            while (game.aliens[ai].type == ALIEN_DEAD) {
                ++ai;
            }
            const Sprite& sprite = alien_sprites[2 * (game.aliens[ai].type - 1)];
            size_t pos = game.aliens[ai].x - (alien_death_sprite.width - sprite.width) / 2;
            if (pos > alien_swarm_position) {
                alien_swarm_position = pos;
            }

            ai = game.num_aliens - 1;
            while (game.aliens[ai].type == ALIEN_DEAD) {
                --ai;
            }
            pos = game.width - game.aliens[ai].x - 13 + pos;
            if (pos > alien_swarm_max_position) {
                alien_swarm_max_position = pos;
            }
        } else {
            alien_update_frequency = 120;
            alien_swarm_position = 24;

            game.aliens_killed = 0;
            alien_update_timer = 0;

            alien_move_dir = 4;

            for (size_t xi = 0; xi < 11; ++xi) {
                for (size_t yi = 0; yi < 5; ++yi) {
                    size_t ai = xi * 5 + yi;
                    game.death_counters[ai] = 10;

                    Alien& alien = game.aliens[ai];
                    alien.type = (5 - yi) / 2 + 1;

                    const Sprite& sprite = alien_sprites[2 * (alien.type - 1)];

                    alien.x = 16 * xi + alien_swarm_position + (alien_death_sprite.width - sprite.width) / 2;
                    alien.y = 16 * yi + 128;
                }
            }
        }

        // Process events
        if (fire_pressed && game.num_bullets < GAME_MAX_BULLETS) {
            game.bullets[game.num_bullets].x = game.player.x + player_sprite.width / 2;
            game.bullets[game.num_bullets].y = game.player.y + player_sprite.height;
            game.bullets[game.num_bullets].dir = 2;
            ++game.num_bullets;
        }
        fire_pressed = false;

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    glDeleteVertexArrays(1, &fullscreen_triangle_vao);

    for (size_t i = 0; i < 6; ++i) {
        delete[] alien_sprites[i].data;
    }
    delete[] text_spritesheet.data;
    delete[] alien_death_sprite.data;
    delete[] player_bullet_sprite.data;
    delete[] alien_bullet_sprite[0].data;
    delete[] alien_bullet_sprite[1].data;
    delete[] alien_bullet_animation.frames;

    for (size_t i = 0; i < 3; ++i) {
        delete[] alien_animation[i].frames;
    }


    return EXIT_SUCCESS;
}
