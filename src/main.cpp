#include <argumentsea.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <sstream>
#include <string>

char* orbital_file = NULL;
char* args_file = NULL;
char* homedir = NULL;

std::string read() {
    std::ifstream file (args_file, std::ios::in);

    std::string buffer = "";

    std::string line;
    while (std::getline(file, line)) {
        line += '\n';
        buffer.append(line);
    }
    file.close();

    return buffer;
}
int returncode(std::string c) { return WEXITSTATUS(system(c.c_str())); }
int* get_rgb(std::string img) {
    FILE* fp = popen(std::format("main-color {} \"{}\"", (img.ends_with(".png") ? "-png" : "-jpg"), img).c_str(), "r");

    char buffer[13];
    std::string rgb_spaces;
    while (fgets(buffer, 13, fp)) { rgb_spaces += buffer; }

    std::istringstream rgb_stream(rgb_spaces);
    int* rgb = new int[3];
    int n = 0, it = 0;

    while (rgb_stream >> n) {
        if (it > 3) break;
        rgb[it] = n;
        it++;
    }

    pclose(fp);

    return rgb;
}

void catcher(ArgumentsManager* manager) {
    const std::string args = read();

    if (returncode(std::format("swww img {} {}", manager->get_argument(manager->get_index()), args)) != 0) {
        fprintf(stderr, "swww failed");
        exit(EXIT_FAILURE);
    }
    for (const std::filesystem::directory_entry entry : std::filesystem::directory_iterator((std::string){homedir} + "/.local/state/swww-general-extensions")) {
        if (entry.path().string().ends_with(".py")) {
            if (std::filesystem::exists("/usr/bin/python")) system(std::format("python \"{}\" \"{}\"", entry.path().string(), manager->get_argument(manager->get_index())).c_str());
            else system(std::format("python3 \"{}\" \"{}\"", entry.path().string(), manager->get_argument(manager->get_index())).c_str());
        }
    }

    exit(EXIT_SUCCESS);
}
int main(int argc, char** argv) {
    ArgumentsManager manager(argv, argc, 1);

    // C-Style
    homedir = getenv("HOME");
    if (!homedir) {
        fprintf(stderr, "Lil bro is using templeos, Anyways, The environment variable HOME dosen't exists gng");
        return EXIT_FAILURE;
    }

    const char* path = ".local/state/initback.sh";
    const size_t length = (strlen(homedir) + strlen(path)) + 2;
    orbital_file = new char[length];
    snprintf(orbital_file, length, "%s/%s", homedir, path);
    const char* path_args = ".local/state/swww-setter-args";
    const size_t length_args = (strlen(homedir) + strlen(path_args)) + 2;
    args_file = new char[length_args];
    snprintf(args_file, length_args, "%s/%s", homedir, path_args);

    if (!std::filesystem::exists(orbital_file)) (std::ofstream(orbital_file));
    if (!std::filesystem::exists(args_file)) (std::ofstream(args_file));
    if (!std::filesystem::exists((std::string){homedir} + "/.local/state/swww-general-extensions"))
        std::filesystem::create_directories((std::string){homedir} + "/.local/state/swww-general-extensions");

    manager.set_catcher(catcher);

    manager.run();
    
    delete[] orbital_file;
    delete[] args_file;

    return EXIT_SUCCESS;
}