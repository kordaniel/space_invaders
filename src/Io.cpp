#include "Io.h"

void io::print_to_stdout(const std::string &msg)
{
    std::cout << msg << std::endl;
}

void io::print_to_stderr(const std::string &err_msg)
{
    std::cerr << err_msg << std::endl;
}

bool
io::readTextFileIntoVector(const std::string& filepath,
                           std::vector<std::string>& buffer,
                           bool includeEmptylines)
{
    std::ifstream in(filepath);
    if (!in) {
        print_to_stderr_varargs("[ERROR]: reading file: ", filepath);
        return false;
    }

    std::string line;

    while (std::getline(in, line)) {
        if (line.empty() && !includeEmptylines) {
            continue;
        }
        buffer.push_back(line);
    }

    return true;
}
