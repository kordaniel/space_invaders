#include "Global.hpp"

#include "Timer.hpp"
#include "Io.hpp"
#include "Buffer.hpp"
#include "Game.hpp"

#include <cstring> // strcmp
#include <cstdlib> // srand, rand


void initialize([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
#ifndef NDEBUG
    if (argc > 1) {
        if (strcmp(argv[1], "-log") == 0) {
            Logger::EnableFileOutput(SI::FILEPATHS::FILE_LOG);
        } else {
            io::print_to_stdout_varargs("Invalid argument: '", argv[1], "'.");
            io::print_to_stdout_varargs("Use '-log' to enable logging into the file '", SI::FILEPATHS::FILE_LOG, "'.");
        }
    }
    Logger::Debug("Starting in DEBUG-mode. Built with:");
    Logger::Debug("Compiler name:       %s.", COMPILER_NAME);
    Logger::Debug("Compiler ID:         %s.", COMPILER_ID);
    Logger::Debug("Compiler version:    %s.", COMPILER_VERSION);
    Logger::Debug("Glew configuration:  %s.", GLEWLIB);
    Logger::Debug("Glfw3 configuration: %s.", GLFWLIB);
#endif

    Logger::Info("Space Invaders version: %.2f. Loading..", SI::GLOBAL::VERSION);
    srand(time(nullptr));

    Logger::Info("Loading completed!");
}

int main(int argc, char* argv[])
{
#ifndef NDEBUG
    Timer timer("Total runtime: ");
#endif

    initialize(argc, argv);

    const int32_t buff_width = 224, buff_height = 256;

    Buffer buffer(buff_width, buff_height);
    Game game(buff_width, buff_height);

    glfwSetWindowUserPointer(buffer.get_glfw_window(), &game);

    game.Run(buffer);

    Logger::Info("Space Invaders version: %.2f. Exiting..", SI::GLOBAL::VERSION);

    return EXIT_SUCCESS;
}
