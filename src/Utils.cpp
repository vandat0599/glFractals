#include "Utils.hpp"

#include <fstream>
#include <sstream>
#include <string>

auto fileToString(const std::string& path) -> std::string
{
    std::ifstream file;
    std::stringstream sBuf;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // May throw, but let caller handle.
    file.open(path);
    sBuf << file.rdbuf();
    return sBuf.str();
}