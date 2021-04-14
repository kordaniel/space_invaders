#include "Buffer.h"

// Section with 4 functions that should not be used outside this module
// ----------------------------------------------------------------------
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Game *game = reinterpret_cast<Game *>(glfwGetWindowUserPointer(window));

    switch (key) {
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS) {
                game->player.SetDirectionRight(true);
            } else if (action == GLFW_RELEASE) {
                game->player.SetDirectionRight(false);
            }
            break;
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS) {
                game->player.SetDirectionLeft(true);
            } else if (action == GLFW_RELEASE) {
                game->player.SetDirectionLeft(false);
            }
            break;
        case GLFW_KEY_UP:
            if (action == GLFW_PRESS) {
                game->player.SetDirectionUp(true);
            } else if (action == GLFW_RELEASE) {
                game->player.SetDirectionUp(false);
            }
            break;
        case GLFW_KEY_DOWN:
            if (action == GLFW_PRESS) {
                game->player.SetDirectionDown(true);
            } else if (action == GLFW_RELEASE) {
                game->player.SetDirectionDown(false);
            }
            break;
        case GLFW_KEY_SPACE:
            game->create_player_bullet();
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
    io::print_to_stdout("Window close callback called!");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    io::print_to_stdout_varargs("Framebuffer size: ", width, ", ", height);
}

void error_callback(int error, const char* description)
{
    io::print_to_stderr_varargs("[ERROR]: ", description);
}
// ----------------------------------------------------------------------


Buffer::Buffer(int32_t width, int32_t height):
    Size(width, height),
    window_title("Space Invaders! FPS:     "),
    time_prev_update(std::chrono::steady_clock::now()),
    n_frames(0),
    fps_prev(0)
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
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
    glDeleteVertexArrays(1, &fullscreen_triangle_vao);
    delete[] data;
}

void Buffer::clear(void)
{
    clear(colors::BG_GREEN);
}

void Buffer::clear(uint32_t color)
{
    for (size_t i = 0; i < width * height; ++i) {
        data[i] = color;
    }
}

void Buffer::append_object(Spaceobject& obj, colors::Colors color)
{
    const int32_t &spr_x      = obj.x;
    const int32_t &spr_y      = obj.y;
    const int32_t &spr_width  = obj.obj_sprite.width;
    const int32_t &spr_height = obj.obj_sprite.height;
    const uint8_t* sprite     = obj.obj_sprite.data;

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
            if (!sprite[yi * spr_width + xi]) {
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
    if (!y_is_in_bounds(y)) {
        return;
    }

    const int32_t y_start = compute_y_start_indx(y);
    for (int32_t x = 0; x < width; ++x) {
        data[y_start + x] = color;
    }
}

void Buffer::draw(void)
{
    update_fps();
    glTexSubImage2D(
        GL_TEXTURE_2D, 0, 0, 0,
        width, height,
        GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
        data
    );
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glfwSwapBuffers(glfw_window);
}

GLFWwindow* Buffer::get_glfw_window(void)
{
    return glfw_window;
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
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

    gl_debug(__FILE__, __LINE__);

    io::print_to_stdout_varargs("Using OpenGL: ", glVersion[0], ".", glVersion[1]);
    io::print_to_stdout_varargs("Renderer used: ", glGetString(GL_RENDERER));
    io::print_to_stdout_varargs("Shading language: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

    //glfwSwapInterval(0); // vsync OFF
    glfwSwapInterval(1); // vsync ON
    glClearColor(1.0, 0.0, 0.0, 1.0);
}

void Buffer::initialize_buffer_texture(void)
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

void Buffer::initialize_shaders(void)
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
        io::print_to_stderr("[ERROR]: Validating shader.");
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

void Buffer::init_shader(const char *shader_code, GLenum type)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &shader_code, nullptr);
    glCompileShader(shader);
    validate_shader(shader, shader_code);
    glAttachShader(shader_id, shader);

    glDeleteShader(shader);
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

int32_t Buffer::compute_sprite_yx_start_indx(const int32_t& x, const int32_t& y, const int32_t& spr_height)
{
    return width * (spr_height - 1 + y) + x;
}

int32_t Buffer::compute_y_start_indx(const int32_t &y)
{
    return width * y;
}

void Buffer::update_fps(void)
{
    // TODO: Maybe add correction if time_delta grows "too large"..?
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

    io::print_to_stdout_varargs("FPS: ", n_frames, ".\tElapsed time: ", time_delta.count(), "s.");

    for (size_t i = 24; i > 20; --i) {
        window_title[i] = (char) (n_frames % 10 + 48);
        n_frames /= 10;
    }
    assert(n_frames == 0);
    glfwSetWindowTitle(glfw_window, window_title);
}
