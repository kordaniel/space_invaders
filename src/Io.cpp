#include "Io.h"

void io::print_to_stdout(const std::string &msg)
{
    std::cout << msg << std::endl;
}

void io::print_to_stderr(const std::string &err_msg)
{
    std::cerr << err_msg << std::endl;
}
