#include "Buffer.h"

#ifdef DEBUG
// First 2 functions will integrate with the IDE to set breakpoints to help debugging.
// MSVC: __debugbreak()
// Xcode: __builtin_debugtrap() or __builtin_trap()
// Posix: #include <signal.h> => raise(SIGTRAP) This should work with gdb
    #include <signal.h>
    #define ASSERT(x) if (!(x)) raise(SIGTRAP);
    #define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))

    static void GLClearError() { while (glGetError() != GL_NO_ERROR); }
    static bool GLLogCall(const char* function, const char* file, int line)
    {
        while (GLenum error = glGetError()) {
            // NOTE: We check for errors in a while loop since there might be several errors in the queue.
            //       !!!! To keep things simple, we only print the first error before exiting for now !!!!
            io::print_to_stderr_varargs("[OpenGL Error] (", error, "): ", function, " ", file, ":", line);
            return false;
        }
        return true;
    }
#else
    #define GLCall(x) x
#endif

// Section with 4 functions that should not be used outside this module
// ----------------------------------------------------------------------
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    assert(scancode == scancode && mods == mods);
    Game *game = reinterpret_cast<Game *>(glfwGetWindowUserPointer(window));

    switch (key) {
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS) {
                game->getPlayer().SetDirectionRight(true);
            } else if (action == GLFW_RELEASE) {
                game->getPlayer().SetDirectionRight(false);
            }
            break;
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS) {
                game->getPlayer().SetDirectionLeft(true);
            } else if (action == GLFW_RELEASE) {
                game->getPlayer().SetDirectionLeft(false);
            }
            break;
        case GLFW_KEY_UP:
            if (action == GLFW_PRESS) {
                game->getPlayer().SetDirectionUp(true);
            } else if (action == GLFW_RELEASE) {
                game->getPlayer().SetDirectionUp(false);
            }
            break;
        case GLFW_KEY_DOWN:
            if (action == GLFW_PRESS) {
                game->getPlayer().SetDirectionDown(true);
            } else if (action == GLFW_RELEASE) {
                game->getPlayer().SetDirectionDown(false);
            }
            break;
        case GLFW_KEY_SPACE:
            if (action == GLFW_RELEASE) {
                game->create_player_bullet();
            }
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_F:
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 2560, 1440, 60);
            break;
        case GLFW_KEY_G:
            glfwSetWindowMonitor(window, nullptr, 0, 0, 224, 256, 60);
            break;
        default:
            break;
    }
}

void window_close_callback(GLFWwindow* window)
{
    assert(window == window);
    io::print_to_stdout("Window close callback called!");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    assert(window == window);
    io::print_to_stdout_varargs("Framebuffer size: ", width, ", ", height);
}

void error_callback(int error, const char* description)
{
    assert(error == error);
    io::print_to_stderr_varargs("[ERROR]: ", description);
}
// ----------------------------------------------------------------------

Buffer::Buffer(int32_t bufferWidth, int32_t bufferHeight, Sprites& sprites)
    : Size(bufferWidth, bufferHeight)
    , m_sprites(sprites)
    , window_title("Space Invaders! FPS:     ")
    , time_prev_update(std::chrono::steady_clock::now())
    , n_frames(0)
    , fps_prev(0)
{
    data = new uint32_t[width * height];
    clear();

    initialize_glfw_window();
    initialize_opengl();
    initialize_buffer_texture();
    initialize_shaders();
}

Buffer::~Buffer(void)
{
    GLCall(glfwDestroyWindow(glfw_window));
    GLCall(glfwTerminate());
    GLCall(glDeleteProgram(shader_id));
    GLCall(glDeleteVertexArrays(1, &fullscreen_triangle_vao));
    delete[] data;
}

void Buffer::clear(void)
{
    clear(colors::BG_GREEN);
}

void Buffer::clear(uint32_t color)
{
    for (int32_t i = 0; i < getTotalSize(); ++i) {
        data[i] = color;
    }
}

void Buffer::drawObject(Spaceobject& obj, colors::Colors color)
{
    const int32_t& spr_x      = obj.x;
    const int32_t& spr_y      = obj.y;
    const Sprite& spriteObj   = m_sprites.getSprite(obj.m_spriteType, obj.m_spriteSelector);
    const int32_t& spr_width  = spriteObj.width;
    const int32_t& spr_height = spriteObj.height;

    int32_t y_startidx;

    for (int32_t yi = 0; yi < spr_height; ++yi) {
        #ifdef DEBUG
            if (!y_is_in_bounds(spr_height - 1 + spr_y - yi)) {
                io::print_to_stdout_varargs("Trying to draw outofbounds Y!!", (spr_height - 1 + spr_y - yi));
                continue;
            }
        #endif
        y_startidx = compute_sprite_yx_start_indx(spr_x, (spr_y - yi), spr_height);
        for (int32_t xi = 0; xi < spr_width; ++xi) {
            if (!spriteObj.data[yi * spr_width + xi]) {
                continue;
            }
            
            #ifdef DEBUG
                else if (!x_is_in_bounds(spr_x + xi)) {
                    io::print_to_stdout("Trying to draw outofbounds X!!");
                    io::print_to_stdout_varargs(spr_x, ", ", xi, " = ", (spr_x + xi));
                    continue;
                }
            #endif
            
            data[y_startidx + xi] = color;
        }
    }
}

void Buffer::append_horizontal_line(int32_t y, colors::Colors color) {
    #ifdef DEBUG
    if (!y_is_in_bounds(y)) {
        return;
    }
    #endif

    const int32_t y_start = compute_y_start_indx(y);
    for (int32_t x = 0; x < width; ++x) {
        data[y_start + x] = color;
    }
}

void Buffer::append_text(int32_t x, int32_t y,
                         const Sprite& text_spritesheet,
                         const std::string& text,
                         colors::Colors color)
{
    const int32_t stride       = text_spritesheet.getTotalSize();
    const int32_t str_width    = text.length() * (text_spritesheet.width + character_gap) - character_gap;
    const uint8_t* sprite_char = text_spritesheet.data;
    char currChar              = 0;
    int32_t xpos               = x + str_width < width ? x : width - str_width;

    for (auto &ch : text) {
        currChar = ch - 32;

        #ifdef DEBUG
        if (currChar < 0 || currChar > 65) {
            io::print_to_stdout_varargs("[ERROR]: Unable to draw character: ", currChar);
            continue;
        }
        #endif
        sprite_char = text_spritesheet.data + currChar * stride;
        append_sprite(xpos, y, sprite_char, text_spritesheet.width, text_spritesheet.height, color);
        xpos += text_spritesheet.width + character_gap;
    }
}

void Buffer::append_integer(int32_t x, int32_t y, Sprite& text_spritesheet, int32_t number, colors::Colors color)
{
    assert(number >= 0);

    if (number == 0) {
        if (width < x + text_spritesheet.width) {
            x = width - text_spritesheet.width;
        }
        append_sprite(x, y, text_spritesheet.getNumberSpritePtr(0),
                      text_spritesheet.width, text_spritesheet.height,
                      colors::Colors::ORANGE
        );
        return;
    }

    append_digits(x, y, text_spritesheet, number, 0, color);
}

void Buffer::draw(void)
{
    GLCall(glTexSubImage2D(
        GL_TEXTURE_2D, 0, 0, 0,
        width, height,
        GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
        data
    ));
    GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
    GLCall(glfwSwapBuffers(glfw_window));
    update_fps();
}

GLFWwindow* Buffer::get_glfw_window(void)
{
    return glfw_window;
}

// ----------------------------- PRIVATE METHODS -----------------------------

ShaderProgramSource Buffer::read_and_parse_shader(const std::string& filepath)
{
    enum ShaderType: int
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::vector<std::string> shadersRawSource;
    bool read_success = io::readTextFileIntoVector(filepath, shadersRawSource, false);
    if (!read_success) {
        io::print_to_stdout("Falling back to default shaders!");
        return { };
    }

    std::stringstream shaders[2];
    ShaderType type = ShaderType::NONE;

    for (auto &rawLine : shadersRawSource) {
        if (rawLine.find("#shader") != std::string::npos) {
            if (rawLine.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (rawLine.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            } else {
                // Syntax error in shaderfile
                //  => Let the program crash if there are more lines to parse.
                io::print_to_stderr_varargs("[ERROR]: Syntax error in shaderfile: ", filepath);
                type = ShaderType::NONE;
            }
        } else {
            shaders[type] << rawLine << '\n';
        }
    }

    return { shaders[ShaderType::VERTEX].str(), shaders[ShaderType::FRAGMENT].str() };
}

void Buffer::initialize_glfw_window(void)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        io::print_to_stderr("[ERROR]: when trying to initialize glfw.");
        delete[] data;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfw_window = glfwCreateWindow(width, height, window_title, nullptr, nullptr);
    if (!glfw_window) {
        io::print_to_stderr("[ERROR]: when trying to create glfw window.");
        delete[] data;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowCloseCallback(glfw_window, window_close_callback);
    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);
    glfwSetKeyCallback(glfw_window, key_callback);
    glfwMakeContextCurrent(glfw_window);
    err = glewInit();
    if (err != GLEW_OK) {
        io::print_to_stderr("[ERROR]: initializing GLEW.");
        io::print_to_stderr_varargs(glewGetErrorString(err));
        delete[] data;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

void Buffer::initialize_opengl(void)
{
    int glVersion[2] = {-1, 1};
    GLCall(glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]));
    GLCall(glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]));

    GLCall(gl_debug(__FILE__, __LINE__));

    io::print_to_stdout_varargs("Using OpenGL: ", glVersion[0], ".", glVersion[1]);
    io::print_to_stdout_varargs("Renderer used: ", glGetString(GL_RENDERER));
    io::print_to_stdout_varargs("Shading language: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

    #ifdef DEBUG
    GLCall(glfwSwapInterval(1)); // vsync 1 = ON, 0 = OFF
    #else
    GLCall(glfwSwapInterval(1)); // vsync ON
    #endif

    GLCall(glClearColor(1.0, 0.0, 0.0, 1.0));
}

void Buffer::initialize_buffer_texture(void)
{
    GLCall(glGenTextures(1, &buffer_texture));
    GLCall(glBindTexture(GL_TEXTURE_2D, buffer_texture));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0,
                GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data
    ));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
}

void Buffer::initialize_shaders(void)
{
    ShaderProgramSource shaderSource = read_and_parse_shader("res/shaders/Shaders.shader");
    GLCall(glGenVertexArrays(1, &fullscreen_triangle_vao));

    shader_id = glCreateProgram();

    compile_shader(GL_VERTEX_SHADER, shaderSource.VertexSource.c_str());
    compile_shader(GL_FRAGMENT_SHADER, shaderSource.FragmentSource.c_str());

    GLCall(glLinkProgram(shader_id));

    if (!validate_program(shader_id)) {
        io::print_to_stderr("[ERROR]: Validating shader.");
        glfwTerminate();
        glDeleteVertexArrays(1, &fullscreen_triangle_vao);
        delete[] data;
        exit(EXIT_FAILURE);
    }

    GLCall(glUseProgram(shader_id));

    location = glGetUniformLocation(shader_id, "buffer");
    GLCall(glUniform1i(location, 0));

    GLCall(glDisable(GL_DEPTH_TEST));
    GLCall(glActiveTexture(GL_TEXTURE0));

    GLCall(glBindVertexArray(fullscreen_triangle_vao));
}

void Buffer::compile_shader(GLenum type, const char *shader_sourcecode)
{
    GLuint shader = glCreateShader(type);

    GLCall(glShaderSource(shader, 1, &shader_sourcecode, nullptr));
    GLCall(glCompileShader(shader));
    GLCall(validate_shader(shader, shader_sourcecode));
    GLCall(glAttachShader(shader_id, shader));

    GLCall(glDeleteShader(shader));
}

void Buffer::validate_shader(GLuint shader, const char *file = nullptr)
{
    // Allocate dynamic char[] buffer on the stack to make sure we have room for the full message
    //
    //int length_on_stack;
    //glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length_on_stack);
    //char* message = (char*)alloca(length_on_stack * sizeof(char));
    //glGetShaderInfoLog(shader, length_on_stack, &length_on_stack, message);
    // ...

    static const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
    if (length > 0) {
        io::print_to_stderr_varargs("[ERROR]: Shader ", shader,
                                    "(", (file? file: ""),
                                    ") compile error: ", buffer
        );
    }
}

bool Buffer::validate_program(GLuint program)
{
    static const GLsizei BUFFER_SIZE = 512;
    GLchar buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
    if (length > 0) {
        io::print_to_stderr_varargs("[ERROR]: Program ", program, " link error: ", buffer);
        return false;
    }

    return true;
}

bool Buffer::y_is_in_bounds(const int32_t& y)
{
    return 0 <= y && y < height;
}

bool Buffer::x_is_in_bounds(const int32_t& x)
{
    return 0 <= x && x < width;
}

bool Buffer::pixel_is_in_bounds(const int32_t& x, const int32_t& y)
{
    return x_is_in_bounds(x) && y_is_in_bounds(y);
}

inline int32_t Buffer::compute_sprite_yx_start_indx(const int32_t& x, const int32_t& y, const int32_t& spr_height)
{
    return width * (spr_height - 1 + y) + x;
}

inline int32_t Buffer::compute_y_start_indx(const int32_t &y)
{
    return width * y;
}

void Buffer::append_sprite(int32_t x, int32_t y, const uint8_t* sprite,
                           int32_t spr_width,int32_t spr_height, colors::Colors color)
{
    int32_t y_startidx;
    for (int32_t yi = 0; yi < spr_height; ++yi) {
        y_startidx = compute_sprite_yx_start_indx(0, (y - yi), spr_height);
        for (int32_t xi = 0; xi < spr_width; ++xi) {
            if (sprite[yi * spr_width + xi])
                data[y_startidx + x + xi] = color;
        }
    }
}

int32_t Buffer::append_digits(int32_t x, int32_t y,
                              Sprite& text_spritesheet,
                              int32_t number, size_t digits, colors::Colors color)
{
    if (number > 0) {
        x = append_digits(x, y, text_spritesheet, number / 10, ++digits, color);
    } else if (number == 0) {
        const int32_t num_width = digits * (text_spritesheet.width + character_gap) - character_gap;
        return x + num_width < width ? x : width - num_width;
    }

    append_sprite(x, y, text_spritesheet.getNumberSpritePtr(number % 10),
                  text_spritesheet.width, text_spritesheet.height, color
    );
    return x + text_spritesheet.width + character_gap;
}



void Buffer::update_fps(void)
{
    using namespace std::chrono;
    ++n_frames;
    time_point<steady_clock> time_now = steady_clock::now();
    duration<double> time_delta = time_now - time_prev_update;
    if (time_delta < seconds(1)) {
        return;
    }

    time_prev_update = time_now;

    if (n_frames == fps_prev) {
        n_frames = 0;
        return;
    }

    fps_prev = n_frames;

    io::print_to_stdout_varargs("FPS: ", (n_frames / time_delta.count()));

    for (size_t i = 24; i > 20; --i) {
        window_title[i] = (char) (n_frames % 10 + 48);
        n_frames /= 10;
    }

    assert(n_frames == 0);
    GLCall(glfwSetWindowTitle(glfw_window, window_title));
}
