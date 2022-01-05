# Tested to work with cmake 3.16. Might work with an older version.
cmake_minimum_required(VERSION 3.16)

# Specify which compiler to use
#set(CMAKE_CXX_COMPILER "g++-11")

# Define min required versions of required libraries
set(MIN_REQUIRED_GLEW "2.2")
set(MIN_REQUIRED_GLFW "3.3")

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
if("${GLEW_VERSION}" LESS "${MIN_REQUIRED_GLEW}")
    # Macos/brew CMakeconfig for glew does not set the version so this check will never trigger.
    # ##################
    # USE version 2.2+!!
    # ##################
    message(WARNING "Found GLEW Version ${GLEW_VERSION} which is older than the oldest supported version: ${MIN_REQUIRED_GLEW}. Consider upgrading glew for best experience.")
endif()

find_package(glfw3 "${MIN_REQUIRED_GLFW}" QUIET)

# Compile glfw from submodule if shared glfw library is not found or the found version is too low
if(NOT glfw3_FOUND OR "${glfw3_VERSION}" LESS "${MIN_REQUIRED_GLFW}")
    message(STATUS "Shared systemlib GLFW was not found or its version was too low. Attempting to use the current version from glfw-repository")
    find_package(Git) # QUIET
    if(Git_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
        option(Git_SUBMODULE "Check submodules during build" ON)
        if(Git_SUBMODULE)
            message(STATUS "Submodule GLFW update")
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            RESULT_VARIABLE GIT_SUBMOD_RESULT)
            if(NOT GIT_SUBMOD_RESULT EQUAL "0")
                message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMODULE_RESULT}, please checkout submodules")
            endif()
            set(GLFW_BUILD_TESTS OFF)
            set(GLFW_BUILD_DOCS OFF) # Requires doxygen
            set(glfw3_DIR "${PROJECT_SOURCE_DIR}/external/glfw")
            add_subdirectory("${PROJECT_SOURCE_DIR}/external/glfw")
        else()
            message("Git submodule option set to OFF")
        endif()
    else()
        message(FATAL_ERROR "Git not found: failed to download glfw sources.")
    endif()
else()
    message(STATUS "Using systemwide shared glfw library: ${glfw3_VERSION}")
endif()

# Only build tests in Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/6b74da4757a549563d7c37c8fae3e704662a043b.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Main app
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

configure_file("${CMAKE_SOURCE_DIR}/configuration/Config.hpp.in" "${PROJECT_BINARY_DIR}/src/Config.hpp" @ONLY)

# Only build tests in Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()

    add_subdirectory("${PROJECT_SOURCE_DIR}/test")
    configure_file("${CMAKE_SOURCE_DIR}/configuration/Config.hpp.in" "${PROJECT_BINARY_DIR}/test/Config.hpp" @ONLY)
endif()
