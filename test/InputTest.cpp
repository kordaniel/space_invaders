#include "gtest/gtest.h"
#include "Input.hpp"

TEST(InputTests, KeyPressedInitializedFalse)
{
    for (int keycode : KeyInput::SUPPORTED_KEYS)
    {
        EXPECT_FALSE(KeyInput::GetInstance().IsPressed(keycode)) << "Keycode: " << keycode;
    }
}

TEST(InputTests, KeyPressedReturnsTrue)
{
    KeyInput keyinput;
    for (int keycode : KeyInput::SUPPORTED_KEYS)
    {
        if (keycode == GLFW_KEY_ESCAPE
            || keycode == GLFW_KEY_F
            || keycode == GLFW_KEY_G)
        {
            continue; // These are handled directly by the callback and have no status.
        }
        KeyInput::MainKeyCallback(nullptr, keycode, 0, GLFW_PRESS, 0);
        EXPECT_TRUE(KeyInput::GetInstance().IsPressed(keycode)) << "Keycode: " << keycode;
        // Assert that the status in instantiated object is not changed:
        ASSERT_FALSE(keyinput.IsPressed(keycode)) << "Object / Keycode: " << keycode;
    }
}

TEST(InputTests, KeyPressedReleasedReturnsFalse)
{
    for (int keycode : KeyInput::SUPPORTED_KEYS)
    {
        if (keycode == GLFW_KEY_ESCAPE
            || keycode == GLFW_KEY_F
            || keycode == GLFW_KEY_G)
        {
            continue; // These are handled directly by the callback and have no status.
        }
        KeyInput::MainKeyCallback(nullptr, keycode, 0, GLFW_PRESS, 0);
        EXPECT_TRUE(KeyInput::GetInstance().IsPressed(keycode)) << "Keycode: " << keycode;
        KeyInput::MainKeyCallback(nullptr, keycode, 0, GLFW_RELEASE, 0);
        EXPECT_FALSE(KeyInput::GetInstance().IsPressed(keycode)) << "Keycode: " << keycode;
    }
}
