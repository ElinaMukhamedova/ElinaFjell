#include "Paths.hpp"

std::filesystem::path projectPath() {
    std::string const filePath = std::string(__FILE__);
    auto const pos = filePath.find("src");
    return std::filesystem::path(filePath.substr(0, pos)).make_preferred();
}
std::filesystem::path resourcesPath() { return projectPath() / "resources";}