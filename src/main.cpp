#include "global.h"

#include "Timer.h"
#include "Io.h"
#include "Buffer.h"
#include "Game.h"

#include <cstring> // strcmp
#include <cstdlib> // srand, rand
#include <ctime>   // time_t


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
    Logger::Debug("Starting in DEBUG-mode");
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

    // Ugly hack to limit the speed of the game in DEBUG mode for now..
    // **************
    uint64_t microsecsInSec = 1 * 1000 * 1000;
    uint64_t updatesPerSecHack = 20;
    using std::chrono::time_point;
    using std::chrono::steady_clock;

    time_point<steady_clock> time_prev_update = steady_clock::now();
    time_point<steady_clock> time_now = steady_clock::now();
    //size_t count = 0;

    while (!glfwWindowShouldClose(buffer.get_glfw_window())) {
        time_now = steady_clock::now();
        if ((time_now - time_prev_update) > std::chrono::microseconds(microsecsInSec / updatesPerSecHack)) {
            game.Update(buffer);
            time_prev_update = time_now;
        }
        // End ugly hack!
        // **************

        game.Draw(buffer);
        buffer.draw();
        buffer.clear();

        glfwPollEvents();
    }

    // CLEANUP, yes or no?? nono!
    //glfwSetWindowUserPointer(buffer.get_glfw_window(), nullptr);


    /////printf("X: %zu, Y: %zu\n", game.player.x, game.player.y);
    /////printf("Game width: %zu, height %zu\n", game.width, game.height);

    //std::cin.get();
    //s_Finished = true;
    //worker_thread.join();

    Logger::Info("Space Invaders version: %.2f. Exiting..", SI::GLOBAL::VERSION);

    return EXIT_SUCCESS;
}
