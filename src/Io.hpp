#ifndef IO_HPP
#define IO_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "Global.hpp"
//#include "Sprites.hpp"

//struct RLESprite; // forward declare

namespace io
{

template<typename T>
inline void print_to_stdout_varargs(T t) {
    std::cout << t << std::endl;
}

template<typename T, typename... Args>
inline void print_to_stdout_varargs(T t, Args... args) {
    std::cout << t;
    print_to_stdout_varargs(args...);
}

template<typename T>
inline void print_to_stderr_varargs(T t) {
    std::cerr << t << std::endl;
}

template<typename T, typename... Args>
inline void print_to_stderr_varargs(T t, Args... args) {
    std::cerr << t;
    print_to_stderr_varargs(args...);
}


void print_to_stdout(const std::string &);
void print_to_stderr(const std::string &);

bool readTextFileIntoVector(const std::string& filepath,
                            std::vector<std::string>& buffer,
                            bool includeEmptylines = true);

bool readBinaryFile(const std::string& filepath, std::vector<uint8_t>& buffer);
bool writeBinaryFile(const std::string& filepath, const std::vector<uint8_t>& buffer);

bool readBinary(const std::string& filepath, const char* buffer);

//private:
//    io(void); // Should be used as a static class => private constructor.
}

#endif
