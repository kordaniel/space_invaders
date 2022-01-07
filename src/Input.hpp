#ifndef INPUT_H
#define INPUT_H

#include "Global.hpp"
#include "Window.hpp" // GLFW

#include <unordered_map>
#include <vector>
#include <functional>

class KeyInput
{
public:
    static int constexpr SUPPORTED_KEYS[] = {
        GLFW_KEY_RIGHT,
        GLFW_KEY_LEFT,
        GLFW_KEY_UP,
        GLFW_KEY_DOWN,
        GLFW_KEY_SPACE,
        GLFW_KEY_ESCAPE,
        GLFW_KEY_F,
        GLFW_KEY_Q
    };
    static KeyInput & GetInstance(void);
    static void MainKeyCallback(GLFWwindow * window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods);
    static void RegisterCallback(int key, int action, const std::function<void(void)> callback);
    //static void UnRegisterCallback(int key, int action, const std::function<void(void)> & callback);

    KeyInput(void);
    bool IsPressed(int key) const;

private:
    static void callAllCallables(int key, int action);

    void setKeyStatus(int key, bool isPressed);

private:
    inline static std::unordered_map<int, std::vector<std::function<void(void)>>> s_keyPressCallbacks;
    inline static std::unordered_map<int, std::vector<std::function<void(void)>>> s_keyReleaseCallbacks;

    std::unordered_map<int, bool> m_keyStatus;
};

#endif
