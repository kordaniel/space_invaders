#ifndef BUFFER_H
#define BUFFER_H

#include <cstdlib>
#include <cstdio>
#include <chrono>

#include "global.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Io.h"
#include "Size.h"
#include "Spaceobject.h"
#include "Game.h"


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


// Colors are 32bits, 8 bits each for R,G,B and alpha values.
namespace colors
{
    enum Colors: uint32_t {
        RED      = (uint32_t) (128 << 24) |                          255,
        ORANGE   = (uint32_t) (160 << 24) | (96 << 16)             | 255,
        BG_GREEN = (uint32_t) (50  << 24) | (80 << 16) | (75 << 8) | 255
    };
} // end namespace colors


class Buffer: public Size
{
    public:
        Buffer(int32_t, int32_t);
        ~Buffer(void);
        void clear(void);
        void clear(uint32_t);
        void append_object(Spaceobject&, colors::Colors color = colors::ORANGE);
        void append_horizontal_line(int32_t, colors::Colors color = colors::ORANGE);
        void draw(void);
        GLFWwindow* get_glfw_window(void);
    private:
        uint32_t* data; // 8 bits each for R,G,B,Alpha values
        char window_title[26];
        GLFWwindow *glfw_window;
        GLenum err;
        GLuint buffer_texture;
        GLuint fullscreen_triangle_vao;
        GLuint shader_id;
        GLint location;

        // Used for computing FPS
        std::chrono::time_point<std::chrono::steady_clock> time_prev_update;
        uint16_t n_frames;
        uint16_t fps_prev;
        // --------- | | ---------

        void initialize_glfw_window(void);
        void initialize_opengl(void);
        void initialize_buffer_texture(void);
        void initialize_shaders(void);
        void init_shader(const char*, GLenum);
        void validate_shader(GLuint, const char*);
        bool validate_program(GLuint);
        bool y_is_in_bounds(const int32_t&);
        bool x_is_in_bounds(const int32_t&);
        bool pixel_is_in_bounds(const int32_t&, const int32_t&);
        int32_t compute_sprite_yx_start_indx(const int32_t&, const int32_t&, const int32_t&);
        int32_t compute_y_start_indx(const int32_t&);
        void update_fps(void);
};

#endif
