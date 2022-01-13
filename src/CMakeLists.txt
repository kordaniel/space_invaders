cmake_minimum_required(VERSION 3.16)

set(headers
    "Buffer.hpp"
    "Compression.hpp"
    "Game.hpp"
    "GameObject.hpp"
    "Global.hpp"
    "Input.hpp"
    "Io.hpp"
    "Logger.hpp"
    "Size.hpp"
    "Sprites.hpp"
    "Timer.hpp"
    "Tools.hpp"
    "Window.hpp"
)

set(sources
    "Main.cpp"
    "Buffer.cpp"
    "Compression.cpp"
    "Game.cpp"
    "GameObject.cpp"
    "Input.cpp"
    "Io.cpp"
    "Logger.cpp"
    "Size.cpp"
    "Sprites.cpp"
    "Timer.cpp"
    "Tools.cpp"
    "Window.cpp"
)

add_executable("${PROJECT_NAME}" "${sources}" "${headers}") # EXCLUDE_FROM_ALL

target_compile_options("${PROJECT_NAME}" PRIVATE "${CXX_FLAGS}"
     "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>"
     "$<$<CONFIG:Release>:${CXX_FLAGS_RELEASE}>"
)

target_link_options("${PROJECT_NAME}" PRIVATE "$<$<CONFIG:Debug>:${CXX_LDFLAGS_DEBUG}>")

target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories("${PROJECT_NAME}"
    PRIVATE "${CMAKE_SOURCE_DIR}/external/stb_image"
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries("${PROJECT_NAME}" PUBLIC OpenGL::GL GLEW::GLEW glfw)
target_link_libraries("${PROJECT_NAME}" PUBLIC OpenGL::GL GLEW::GLEW glfw glm::glm)

# Copy the required resources to the output directory
add_custom_command(
    TARGET "${PROJECT_NAME}" POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_SOURCE_DIR}/res/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/res/shaders
    COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_SOURCE_DIR}/res/spr $<TARGET_FILE_DIR:${PROJECT_NAME}>/res/spr
)
