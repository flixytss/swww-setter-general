#include <argumentsea.hpp>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

std::string orbital_file;
std::string args_file;
std::string homedir_str;

std::string read_args() {
    std::ifstream file(args_file);
    if (!file) return "";
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int returncode(const std::string& c) { 
    int status = system(c.c_str());
    return WEXITSTATUS(status); 
}

void catcher(ArgumentsManager* manager) {
    const std::string args = read_args();
    std::string img_path = manager->get_argument(manager->get_index());

    if (returncode(std::format("swww img \"{}\" {}", img_path, args)) != 0) {
        std::cerr << "swww failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    fs::path ext_dir = fs::path(homedir_str) / ".local/state/swww-general-extensions";
    std::string lua_cmd = "lua";
    for (const auto& entry : fs::directory_iterator(ext_dir)) {
        if (entry.path().extension() == ".lua") {
            system(std::format("{} \"{}\" \"{}\"", lua_cmd, entry.path().string(), img_path).c_str());
        }
    }

    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
    ArgumentsManager manager(argv, argc, 1);

    const char* home = getenv("HOME");
    if (!home) {
        std::cerr << "HOME variable not found" << std::endl;
        return EXIT_FAILURE;
    }
    homedir_str = home;

    fs::path base_path = fs::path(homedir_str) / ".local/state";
    orbital_file = (base_path / "initback.sh").string();
    args_file = (base_path / "swww-setter-args").string();
    fs::path extensions_path = base_path / "swww-general-extensions";

    if (!fs::exists(orbital_file)) std::ofstream(orbital_file).close();
    if (!fs::exists(args_file)) std::ofstream(args_file).close();
    if (!fs::exists(extensions_path)) fs::create_directories(extensions_path);

    manager.set_catcher(catcher);
    manager.run();

    return EXIT_SUCCESS;
}