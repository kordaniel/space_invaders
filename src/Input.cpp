#include "Input.hpp"

// PUBLIC functions
// STATIC functions
KeyInput & KeyInput::GetInstance(void)
{
    static KeyInput instance;
    return instance;
}

void KeyInput::MainKeyCallback(GLFWwindow * window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods)
{
    // window is NULL in tests so we need this check to avoid triggering GLFW asserts
    // TODO: Refactor tests to pass ptr to a valid GLFWwindow / Window.cpp object
    Window * windowPtr = window != nullptr
                       ? static_cast<Window*>(glfwGetWindowUserPointer(window))
                       : nullptr;
    callAllCallables(key, action);
    switch(key)
    {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS && windowPtr != nullptr) {
                windowPtr->SetShouldClose(true);
            }
        case GLFW_KEY_Q:
            if (action == GLFW_PRESS && windowPtr != nullptr) {
                windowPtr->SetShouldClose(true);
            }
        case GLFW_KEY_F:
            if (action == GLFW_PRESS && windowPtr != nullptr) {
                windowPtr->SetFullscreen(!windowPtr->IsFullscreen());
            }
        default:
            // Dont break in any previous case => always set the status of every key.
            GetInstance().setKeyStatus(key, action != GLFW_RELEASE);
            break;
    }
}

void KeyInput::RegisterCallback(int key, int action, const std::function<void(void)> callback)
{
    if (action == GLFW_PRESS) {
        s_keyPressCallbacks[key].push_back(callback);
    } else if (action == GLFW_RELEASE) {
        s_keyReleaseCallbacks[key].push_back(callback);
    }
}

// NON-STATIC functions
KeyInput::KeyInput(void)
{
    for (int key : SUPPORTED_KEYS) {
        m_keyStatus[key] = false;
    }
}

bool KeyInput::IsPressed(int key) const
{
    std::unordered_map<int, bool>::const_iterator keyInMap = m_keyStatus.find(key);
    if (keyInMap != m_keyStatus.end())
    {
        return keyInMap->second;
    }
    return false;
}

// PRIVATE functions
// STATIC  functions
void KeyInput::callAllCallables(int key, int action)
{
    using CallbacksMap = std::unordered_map<int, std::vector<std::function<void(void)>>>;
    const CallbacksMap * currMap = nullptr;

    switch (action)
    {
        case GLFW_PRESS:   currMap = &s_keyPressCallbacks; break;
        case GLFW_RELEASE: currMap = &s_keyReleaseCallbacks; break;
        default: return; // This is needed since action is not binary. There can be many alternatives.
    }

    CallbacksMap::const_iterator keyInMap = currMap->find(key);
    if (keyInMap == currMap->end()) {
        return;
    }

    for (const std::function<void(void)> & callable : keyInMap->second) {
        callable();
    }
}

// NON-STATIC functions
void KeyInput::setKeyStatus(int key, bool isPressed)
{
    std::unordered_map<int, bool>::iterator keyInMap = m_keyStatus.find(key);
    if (keyInMap != m_keyStatus.end())
    {
        if (keyInMap->second == isPressed) {
            return;
        }
        keyInMap->second = isPressed;
    }
}
