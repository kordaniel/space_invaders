#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window
{
public:
    Window(const std::string & title, int width, int height, bool vsync = true);
    ~Window(void);

    int  GetXPos(void)           const;
    int  GetYPos(void)           const;
    int  GetWidth(void)          const;
    int  GetHeight(void)         const;
    int  GetViewportWidth(void)  const;
    int  GetViewportHeight(void) const;
    bool IsVsync(void)           const;
    bool IsFullscreen(void)      const;
    bool ShouldClose(void)       const;

    void Update(void);
    void SetVsync(bool enabled);
    void SetTitle(const std::string & title);
    void SetFullscreen(bool fullscreen);
    void SetUpdateViewport(void);
    void SetShouldClose(bool shouldClose);

private:
    static void window_close_callback([[maybe_unused]] GLFWwindow * window);
    static void window_resize_callback(GLFWwindow * window, int width, int height);
    static void window_move_callback(GLFWwindow * window, int xpos, int ypos);
    static void glfw_error_callback(int error, const char* description);
    static GLFWmonitor * get_current_monitor(GLFWwindow * window);

private:
    struct Posdim {
        // Position and dimensions
        int X, Y, Width, Height, ViewportWidth, ViewportHeight;
    };
    struct SavedPosSize {
        int X, Y, Width, Height;
    };

    static bool   s_glfwInitialized;

    Posdim        m_posdim;       // Current position and dimensions
    SavedPosSize  m_savedposdim;  // Position and dimension before fullscreen
    std::string   m_title;
    bool          m_vsync;
    bool          m_updateViewport;
    GLFWwindow  * m_window;
    GLFWmonitor * m_monitor;
};

#endif
