#ifndef IO_H
#define IO_H

#include <iostream>
#include <string>

#include "global.h"

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

} // End namespace io

#endif
