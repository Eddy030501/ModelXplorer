#include "utils.h"
#include <fstream>
#include <sstream>

namespace utils {
    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}
