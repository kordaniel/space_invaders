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
    std::ifstream in(filepath, std::ios::in);
    if (!in.is_open()) {
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

    in.close();
    return true;
}

bool io::readBinaryFile(const std::string& filepath, std::vector<uint8_t>& buffer)
{
    std::ifstream ifs(filepath, std::ios::binary | std::ios::in | std::ios::ate);
    if (!ifs.is_open()) {
        return false;
    }
    //ifs.unsetf(std::ios::skipws); // Skip whitespace

    std::streampos fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    buffer.reserve(fileSize);
    buffer.insert(buffer.begin(),
                  std::istreambuf_iterator<char>(ifs),
                  std::istreambuf_iterator<char>()
    );

    ifs.close();
    if (!ifs.good()) {
        // ERROR. TODO: Handle!!
        return false;
    }

    return true;
}

bool io::writeBinaryFile(const std::string& filepath, const std::vector<uint8_t>& buffer)
{
    if (buffer.size() == 0) {
        return false;
    }

    std::ofstream ofs(filepath, std::ios::binary | std::ios::out); // | std::ios::trunc
    if (!ofs.is_open()) {
        return false;
    }

    ofs.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size() * sizeof(buffer[0]));
    ofs.close();

    if (!ofs.good()) {
        // ERROR. TODO: Handle!!
        return false;
    }

    return true;
}
