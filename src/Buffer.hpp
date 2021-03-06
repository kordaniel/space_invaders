#ifndef BUFFER_H
#define BUFFER_H

#include "Global.hpp"
#include "Size.hpp"
#include "Window.hpp"

#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Sprite;


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

// Struct to hold the shaders sourcecode. Default constructor creates
// default fallback shaders.
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;

    ShaderProgramSource(std::string vertex_src, std::string fragment_src)
        : VertexSource(vertex_src)
        , FragmentSource(fragment_src)
    {
        //
    }
    ShaderProgramSource(void)
        : VertexSource(
R"(
#version 330

noperspective out vec2 TexCoord;

void main(void) {

    TexCoord.x = (gl_VertexID == 2)? 2.0: 0.0;
    TexCoord.y = (gl_VertexID == 1)? 2.0: 0.0;

    gl_Position = vec4(2.0 * TexCoord - 1.0, 0.0, 1.0);
})"
        )
        , FragmentSource(
R"(
#version 330

uniform sampler2D buffer;
noperspective in vec2 TexCoord;

out vec3 outColor;

void main(void) {
    outColor = texture(buffer, TexCoord).rgb;
})"
        )
    {
        //
    }
};


// Colors are 32bits, 8 bits each for R,G,B and alpha values.
namespace colors
{
    enum Colors: uint32_t {
        RED      = (uint32_t) (128 << 24) |                          255,
        ORANGE   = (uint32_t) (160 << 24) | (58 << 16)             | 255,
        BG_GREEN = (uint32_t) (50  << 24) | (48 << 16) | (68 << 8) | 255
    };
} // end namespace colors


class Buffer: public Size
{
public:
    Buffer(int32_t bufferWidth, int32_t bufferHeight, Window & window);
    Buffer(const Buffer& other) = delete;
    ~Buffer(void);
    void clear(void);
    void clear(uint32_t);
    void drawSprite(int32_t x, int32_t y, const Sprite& sprite, colors::Colors color);
    void append_horizontal_line(int32_t, colors::Colors color = colors::ORANGE);
    void append_text(int32_t, int32_t, const Sprite&, const std::string&, colors::Colors = colors::ORANGE);
    void append_integer(int32_t, int32_t, Sprite&, int32_t, colors::Colors color = colors::ORANGE);
    void draw(void);
    bool WindowShouldClose(void) const;

private:
    uint32_t* m_data; // 8 bits each for R,G,B,Alpha values
    char m_window_title[26];
    //GLFWwindow* m_glfw_window;
    Window & m_window;
    GLenum m_err;
    GLuint m_buffer_texture;
    GLuint m_fullscreen_triangle_vao;
    GLuint m_shader_id;
    GLint m_location;
    const static int32_t m_character_gap = 2;// amount of space between chars

    // Used for computing FPS
    std::chrono::time_point<std::chrono::steady_clock> m_time_prev_update;
    uint16_t m_n_frames;
    uint16_t m_fps_prev;
    // --------- |??|??---------

    ShaderProgramSource read_and_parse_shader(const std::string&);
    void initialize_glfw_window(void);
    void initialize_opengl(void);
    void initialize_buffer_texture(void);
    void initialize_shaders(void);
    void compile_shader(GLenum, const char*);
    void validate_shader(GLuint, const char*);
    bool validate_program(GLuint);
    inline bool y_is_in_bounds(int32_t x) const;
    inline bool x_is_in_bounds(int32_t y) const;
    inline bool pixel_is_in_bounds(int32_t x, int32_t y) const;
    inline int32_t compute_sprite_yx_start_indx(int32_t x, int32_t y, int32_t spriteHeigh) const;
    inline int32_t compute_y_start_indx(const int32_t&);
    void append_sprite(int32_t, int32_t, const uint8_t*, int32_t, int32_t, colors::Colors color = colors::ORANGE);
    int32_t append_digits(int32_t, int32_t, Sprite&, int32_t, int32_t, colors::Colors);
    void update_fps(void);
};

#endif
