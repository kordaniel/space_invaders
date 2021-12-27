# Should work with v. 3.15 but has only been tested with v. 3.22.1.
cmake_minimum_required(VERSION 3.15)

# Specify which compiler to use
#set(CMAKE_CXX_COMPILER "g++-11")

# Disable in-source builds.
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
FATAL ERROR: In-source builds are not allowed.
             Specify a separate build dir with the -B flag for build files.
")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

project(SpaceInvaders VERSION 0.6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options("-Wall" "-Wextra" "-Wshadow" "-Wunused-result" "-pedantic")

if(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    add_compile_options("-Wshadow-field-in-constructor" "-Wsign-compare")
#elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
#    message("Compiler: GNU")
endif()

# MacOS default xcode tools doesn't support sanitizers out of the box and
# with the sanitizer flags enabled in the debug target, the 'leaks'-tool doesn't work.
# So use the release build to check for memory leaks in a modern MacOS environment.
list(APPEND CXX_FLAGS_RELEASE "-Werror" "-flto") # -O3 -DNDEBUG is set by cmake
list(APPEND CXX_FLAGS_DEBUG "-fsanitize=undefined" "-fsanitize=address" "-O0") # -g is set by cmake

# At least MacOS clang linking (or only ld do?) requires the fsanitize flags to be set
set(CXX_LDFLAGS_DEBUG "-fsanitize=undefined" "-fsanitize=address" "-rdynamic")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)

#find_library(glewlib glew NAMES glew glew2 GLEW GLEW2 DOC "Glew lib")

#if("${glewlib}" STREQUAL myglewlib-NOTFOUND)
#    message(FATAL_ERROR "[ERROR]: Glew not found!")
#else()
#    message(STATUS "Found Glew: ${glewlib}")
#endif()

add_executable("${PROJECT_NAME}"
    "${PROJECT_SOURCE_DIR}/src/Main.cpp"
    "${PROJECT_SOURCE_DIR}/src/Buffer.cpp"
    "${PROJECT_SOURCE_DIR}/src/Compression.cpp"
    "${PROJECT_SOURCE_DIR}/src/Game.cpp"
    "${PROJECT_SOURCE_DIR}/src/GameObject.cpp"
    "${PROJECT_SOURCE_DIR}/src/Input.cpp"
    "${PROJECT_SOURCE_DIR}/src/Io.cpp"
    "${PROJECT_SOURCE_DIR}/src/Logger.cpp"
    "${PROJECT_SOURCE_DIR}/src/Size.cpp"
    "${PROJECT_SOURCE_DIR}/src/Sprites.cpp"
    "${PROJECT_SOURCE_DIR}/src/Timer.cpp"
    "${PROJECT_SOURCE_DIR}/src/Tools.cpp"
)

include_directories("${PROJECT_NAME}" PRIVATE "${PROJECT_BINARY_DIR}")

configure_file("${CMAKE_SOURCE_DIR}/configuration/Config.hpp.in" Config.hpp @ONLY)


target_compile_options("${PROJECT_NAME}" PRIVATE "${CXX_FLAGS}"
     "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>"
     "$<$<CONFIG:Release>:${CXX_FLAGS_RELEASE}>"
)

target_link_options("${PROJECT_NAME}" PRIVATE
    "$<$<CONFIG:Debug>:${CXX_LDFLAGS_DEBUG}>"
)

target_include_directories("${PROJECT_NAME}" PRIVATE "${PROJECT_BINARY_DIR}/src")

#target_link_libraries("${PROJECT_NAME}" OpenGL::GL ${glewlib} glfw)
target_link_libraries("${PROJECT_NAME}" PUBLIC OpenGL::GL GLEW::GLEW glfw)

# Copy the required resources to the output directory
add_custom_command(
    TARGET "${PROJECT_NAME}" POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_SOURCE_DIR}/res/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/res/shaders
    COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_SOURCE_DIR}/res/spr $<TARGET_FILE_DIR:${PROJECT_NAME}>/res/spr
)
