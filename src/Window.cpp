#include "Window.hpp"
#include "Input.hpp"
#include "Logger.hpp"


bool Window::s_glfwInitialized = false;

Window::Window(const std::string & title, int width, int height, bool vsync)
    : m_posdim({ 0, 0, 0, 0, 0, 0 })
    , m_savedposdim({ 0, 0, 0, 0 })
    , m_title(title)
    , m_vsync(vsync)
    , m_updateViewport(true)
{
    if (!s_glfwInitialized)
    {
        if (!glfwInit())
        {
            Logger::Info("Couldn't initialize GLFW");
            exit(EXIT_FAILURE);
        }
        s_glfwInitialized = true;
        Logger::Debug("Initialized GLFW");
    }

    glfwSetErrorCallback(glfw_error_callback);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(width, height, m_title.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        Logger::Info("Couldn't initialize GLFW window");
        glfwTerminate();
        s_glfwInitialized = false;
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(m_window, this);
    m_monitor = get_current_monitor(m_window);
    glfwGetWindowPos(m_window, &m_posdim.X, &m_posdim.Y);
    glfwGetWindowSize(m_window, &m_posdim.Width, &m_posdim.Height);

    glfwSetWindowCloseCallback(m_window, window_close_callback);
    glfwSetWindowSizeCallback(m_window, window_resize_callback);
    glfwSetWindowPosCallback(m_window, window_move_callback);
    glfwSetKeyCallback(m_window, KeyInput::MainKeyCallback);

    glfwMakeContextCurrent(m_window);
    SetVsync(vsync);
    Logger::Debug("Created window '%s' with the size %d x %d", m_title.c_str(), m_posdim.Width, m_posdim.Height);
    Logger::Debug("Window pos: (%d, %d)", m_posdim.X, m_posdim.Y);
}

Window::~Window(void)
{
    glfwDestroyWindow(m_window);
    if (s_glfwInitialized)
    {
        glfwTerminate();
        s_glfwInitialized = false;
    }
    Logger::Debug("Destroyed window '%s'", m_title.c_str());
}

int  Window::GetXPos(void)           const { return m_posdim.X; }
int  Window::GetYPos(void)           const { return m_posdim.Y; }
int  Window::GetWidth(void)          const { return m_posdim.Width; }
int  Window::GetHeight(void)         const { return m_posdim.Height; }
int  Window::GetViewportWidth(void)  const { return m_posdim.ViewportWidth; }
int  Window::GetViewportHeight(void) const { return m_posdim.ViewportHeight; }
bool Window::IsVsync(void)           const { return m_vsync; }
bool Window::IsFullscreen(void)      const { return glfwGetWindowMonitor(m_window) != nullptr; }
bool Window::ShouldClose(void)       const { return glfwWindowShouldClose(m_window); }

void Window::Update(void)
{
    if (m_updateViewport)
    {
        int * width  = &m_posdim.ViewportWidth;
        int * height = &m_posdim.ViewportHeight;
        glfwGetFramebufferSize(m_window, width, height);
        glViewport(0, 0, *width, *height);
        glfwGetWindowPos(m_window, &m_posdim.X, &m_posdim.Y);
        glfwGetWindowSize(m_window, &m_posdim.Width, &m_posdim.Height);
        m_updateViewport = false;
    }

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::SetVsync(bool enabled)
{
    m_vsync = enabled;
    glfwSwapInterval(m_vsync ? 1 : 0);
}

void Window::SetTitle(const std::string & title)
{
    m_title = title;
    glfwSetWindowTitle(m_window, m_title.c_str());
}

void Window::SetFullscreen(bool fullscreen)
{
    if (IsFullscreen() == fullscreen) {
        return;
    }

    if (fullscreen)
    {
        glfwGetWindowPos(m_window, &m_savedposdim.X, &m_savedposdim.Y);
        glfwGetWindowSize(m_window, &m_savedposdim.Width, &m_savedposdim.Height);
        m_monitor = get_current_monitor(m_window);
        const GLFWvidmode * mode = glfwGetVideoMode(m_monitor);
        glfwSetWindowMonitor(m_window, m_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else
    {
        const GLFWvidmode * mode = glfwGetVideoMode(m_monitor);
        glfwSetWindowMonitor(m_window, nullptr,
                             m_savedposdim.X, m_savedposdim.Y,
                             m_savedposdim.Width, m_savedposdim.Height, mode->refreshRate
        );
    }
}

void Window::SetUpdateViewport(void)
{
    m_updateViewport = true;
}

void Window::SetShouldClose(bool shouldClose)
{
    glfwSetWindowShouldClose(m_window, (shouldClose ? GLFW_TRUE : GLFW_FALSE));
}

void Window::window_close_callback([[maybe_unused]] GLFWwindow * window)
{
    Window * windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowPtr == nullptr) {
        Logger::Debug("GLFW Window close callback called for a window that has no user set -> no action");
        return;
    }
    Logger::Debug("Window (%d) close callback called", window);
}

void Window::window_resize_callback(GLFWwindow * window, [[maybe_unused]] int width, [[maybe_unused]] int height)
{
    Window * windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowPtr == nullptr)
    {
        Logger::Debug("GLFW Window resize callback called for a window that has no user set -> no action");
        return;
    }
    windowPtr->SetUpdateViewport();
}

void Window::window_move_callback(GLFWwindow * window, [[maybe_unused]] int xpos, [[maybe_unused]] int ypos)
{
    Window * windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowPtr == nullptr)
    {
        Logger::Debug("GLFW Window position (move) callback called for a window that has no user set -> no action");
        return;
    }
    windowPtr->SetUpdateViewport();
}

void Window::glfw_error_callback(int error, const char * description)
{
   Logger::Debug("GLFW error (%d): %s", error, description);
}

GLFWmonitor * Window::get_current_monitor(GLFWwindow * window)
{
    int nmonitors, i;
    int wx, wy, ww, wh;
    int mx, my, mw, mh;
    int overlap, bestoverlap;
    GLFWmonitor * bestmonitor;
    GLFWmonitor ** monitors;
    const GLFWvidmode * mode;

    bestoverlap = 0;
    bestmonitor = nullptr;

    glfwGetWindowPos(window, &wx, &wy);
    glfwGetWindowSize(window, &ww, &wh);
    monitors = glfwGetMonitors(&nmonitors);

    for (i = 0; i < nmonitors; i++)
    {
        mode = glfwGetVideoMode(monitors[i]);
        glfwGetMonitorPos(monitors[i], &mx, &my);
        mw = mode->width;
        mh = mode->height;

        overlap = std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx)) *
                  std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

        if (bestoverlap < overlap)
        {
            bestoverlap = overlap;
            bestmonitor = monitors[i];
        }
    }

    return bestmonitor;
}
