#include "Buffer.h"

// Section with two functions that should not be used outside this module
// ----------------------------------------------------------------------

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key) {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS)
                //glfwSetWindowShouldClose(window, true);
                io::print_to_stdout("ESC PRESSED");
            break;
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS)
                //move_dir += 1;
                io::print_to_stdout("right_down");
            else if (action == GLFW_RELEASE)
                io::print_to_stdout("right_up");
                //move_dir -= 1;
            break;
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS)
                io::print_to_stdout("left_down");
                //move_dir -= 1;
            else if (action == GLFW_RELEASE)
                io::print_to_stdout("right_up");
                //move_dir += 1;
            break;
        case GLFW_KEY_SPACE:
            if (action == GLFW_RELEASE)
                io::print_to_stdout("space_up");
                //fire_pressed = true;
            break;
        default:
            break;
    }
}

void error_callback(int error, const char* description)
{
    io::print_to_stderr_varargs("[ERROR]: ", description);
}
// ----------------------------------------------------------------------

Buffer::Buffer(size_t width, size_t height):
    Size(width, height)
{
    data = new uint32_t[width * height];
    clear(0);

    initialize_glfw_window();
    initialize_opengl();
    initialize_buffer_texture();
    initialize_shaders();

    time_prev_update = std::chrono::steady_clock::now();
    n_frames = 0;
}

Buffer::~Buffer(void)
{
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
    glDeleteVertexArrays(1, &fullscreen_triangle_vao);
    delete[] data;
}

void Buffer::clear(uint32_t color)
{
    for (size_t i = 0; i < width * height; ++i) {
        data[i] = color;
    }
}

void Buffer::append_object(Spaceobject& obj)
{
    const uint8_t spr_x = obj.x;
    const uint8_t spr_y = obj.y;
    const uint8_t spr_width  = obj.obj_sprite.width;
    const uint8_t spr_height = obj.obj_sprite.height;
    const uint8_t* sprite    = obj.obj_sprite.data;

    for (size_t xi = 0; xi < spr_width; ++xi) {
        for (size_t yi = 0; yi < spr_height; ++yi) {
            if (sprite[yi * spr_width + xi]
                && (spr_height - 1 + spr_y - yi) < height
                && (spr_x + xi) < width) {
                    data[(spr_height - 1 + spr_y - yi) * width + (spr_x + xi)] = 0;
                }
        }
        //
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

    glfw_window = glfwCreateWindow(width, height, "Space Invaders", nullptr, nullptr);
    if (!glfw_window) {
        io::print_to_stderr("[ERROR]: when trying to create glfw window.");
        delete[] data;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

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

    glfwSwapInterval(0); // vsync OFF
    //glfwSwapInterval(1); // vsync ON
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

    glShaderSource(shader, 1, &shader_code, 0);
    glCompileShader(shader);
    validate_shader(shader, shader_code);
    glAttachShader(shader_id, shader);

    glDeleteShader(shader);
}

void Buffer::validate_shader(GLuint shader, const char *file = nullptr)
{
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
    io::print_to_stdout_varargs("FPS: ", n_frames, ".\tElapsed time: ", time_delta.count(), "s.");
    n_frames = 0;
}
