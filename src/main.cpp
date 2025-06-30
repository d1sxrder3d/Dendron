#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include "./tree.h"
#include "./main.h" 
#include <algorithm>


namespace fs = std::filesystem;

std::string DENDRON_VERSION = "1.1.0";

std::string trim(const std::string& str) {
    const std::string whitespace = " \t";
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; 

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}


//ищет и получает файл конфига 
std::string get_config_path() {
    fs::path config_path;

#if defined(_WIN32)
    const char* appdata = std::getenv("APPDATA");
    if (appdata) {
        config_path = fs::path(appdata) / "dendron" / "config.ini";
    }
#elif defined(__APPLE__)
    const char* home = std::getenv("HOME");
    if (home) {
        config_path = fs::path(home) / "Library" / "Application Support" / "dendron" / "config.ini";
    }
#else 
    const char* home = std::getenv("HOME");
    if (home) {
        config_path = fs::path(home) / ".config" / "dendron" / "config.ini";
    }
#endif

    if (!config_path.empty() && fs::exists(config_path)) {
        return config_path.string();
    }
    // Запасной вариант для разработки 
    return (fs::current_path() / "configs" / "config.ini").string();
}

//
void set_config(ProgramOptions& options){
    std::ifstream config_file(get_config_path());
    if(!config_file.is_open()) {
        
        return;
    }

    std::string line;
    while(std::getline(config_file, line)){
        line = trim(line);
        if(line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }

        size_t pos = line.find('=');
        if(pos == std::string::npos) {
            continue;
        }

        std::string key = trim(line.substr(0, pos));
        std::string value = trim(line.substr(pos + 1));
        

        if(key == "ignore_patterns"){
            std::stringstream ss(value);
            std::string pattern;
            while(std::getline(ss, pattern, ',')){
                std::string trimmed_pattern = trim(pattern);
                if(!trimmed_pattern.empty()){
                    options.ignore_patterns.push_back(trimmed_pattern);
                }
            }

        } else if(key == "default_directory"){
            options.directory_path = value;

        } else if(key == "max_recursion_depth"){
            try{
                int depth = std::stoi(value);
                if(depth >= -1) {
                    options.max_recursion_depth = depth;
                }
            } 
            catch(const std::exception& e){
                std::cerr << "Error: Invalid value of max_recursion_depth config. (safe mode, max_recursion_depth = 1)" << std::endl;
                options.max_recursion_depth = 1;
            }

        } else if(key == "branches_style"){
            try{
                int style = std::stoi(value);
                if(style >= 0 && style < 3) {
                    options.char_style = style;
                } else {
                    throw std::out_of_range("Style out of range");
                }
            } 
            catch(const std::exception& e){
                std::cerr << "Error: Invalid value for config char_style. (safe mode, char_style = 2)" << std::endl;
                options.char_style = 2;
            }

        } else if (key == "show_details") {
            options.show_details = (value == "true");
        } else if (key == "details_format") {
            value.erase(std::remove_if(value.begin(), value.end(), 
                [](char c){ return std::isspace(static_cast<unsigned char>(c)) || c == ','; }), 
                value.end());
            options.details_format = value;

        } else if(key == "tree_style"){
            options.tree_style = (value == "true");

        } else if(key == "ignore_files"){
            options.ignore_files = (value == "true");

        } else if(key == "active_icons"){
            options.active_icons = (value == "true");

        } else if(key == "show_hyperlinks"){
            options.show_hyperlinks = (value == "true");

        } else if (key.rfind("icon_", 0) == 0) { 
            if (key.length() > 5) {
                std::string icon_key = key.substr(5);
                options.icons[icon_key] = value;
            }

        }
    }
}

void set_flags(ProgramOptions& options, int argc, char* argv[]) {

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        
        if (arg == "-r" || arg == "--recursion") {
            if (i + 1 < argc) {
                try {
                    i++; 
                    int depth = std::stoi(argv[i]);
                    if (depth >= -1) {
                        options.max_recursion_depth = depth;
                    } else {
                        throw std::exception();
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: Invalid value for recursion depth. (safe mode, r = 1)" << std::endl;
                    options.max_recursion_depth = 1;
                }
            } else {
                std::cerr << "Error: for argument" << arg << " value required." << std::endl;
            }
        } else if (arg == "-s" || arg == "--style") {
            if (i + 1 < argc) {
                try {
                    i++; // Сначала увеличиваем индекс
                    int style = std::stoi(argv[i]);
                    if (style >= 0 && style < 3) {
                        options.char_style = style;
                    } else {
                        throw std::exception();
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: Invalid value for style. (safe mode, s=2)" << std::endl;
                    options.char_style = 2;
                }
            }
        } else if (arg == "-d" || arg == "--details"){
            options.show_details = true;

        } else if (arg == "--icons") {
            options.active_icons = false;

        } else if (arg == "-t" || arg == "--tree") {
            options.tree_style = true;

        } else if (arg == "-f" || arg == "--files") { 
            options.ignore_files = true;

        } else if (arg == "-i" || arg == "--ignore") {
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                i++;
                options.ignore_patterns.push_back(argv[i]);
            }

        } else if(arg == "--config"){
            options.need_config = true;
            break;

        } else if (arg == "-h" || arg == "--help") {
            options.need_help = true;
            break; 

        } else if (arg == "-v" || arg == "--version") {
            std::cout << "Dendron version: " << DENDRON_VERSION << std::endl;

        } else if (arg == "--iconsoff") {
            options.active_icons = false;

        } else {
            
            
            if (fs::exists(arg)) {
                options.directory_path = arg;
            } else {
                std::cerr << "Unknown argument or invalid path: " << arg << std::endl;
                options.need_help = true;
            }

        }
    }
}





int main(int argc, char* argv[]) {
    // auto start = std::chrono::high_resolution_clock::now();

    ProgramOptions options;

    set_config(options);

    set_flags(options, argc, argv);
    

    if(options.need_config){

#if defined(_WIN32)
#elif defined(__APPLE__)
#else
    std::system("nano ~/.config/dendron/config.ini");
#endif
        return 0;
    }
    if (options.need_help) {
        std::cout << "Usage: ./tree_cli [path] [options]\n"
                  << "Options:\n"
                  << "  -r, --recursion <number>   Set maximum recursion depth\n"
                  << "  -d, --details              Show details of files and directories (permissions, size, etc.)\n"
                  << "  -s, --style <0-2>          Set tree style\n"
                  << "  -t, --tree                 Sort files before directories\n"
                  << "  -f, --files                Ignore files in output\n"
                  << "  -i, --ignore <pattern...>  Ignore files/directories using the pattern\n"
                  << "  -v, --version              Show version\n"
                  << "  --iconsoff <true/false>    Disable icons\n"
                  << "  --config                   Open configuration file\n"
                  << "  -h, --help                 Show this message\n";
        return 0;
    }
    

    if (options.directory_path.empty()) {
        options.directory_path = fs::current_path();
    }

    const fs::path absolute_current_path = fs::canonical(options.directory_path);
    
    std::map<std::string, std::string> icons_by_extension;
    std::string dir_icon;
    std::string file_icon;

    if (options.active_icons) {
        auto dir_it = options.icons.find("dir");
        if (dir_it != options.icons.end()) {
            dir_icon = dir_it->second;
            options.icons.erase(dir_it);
        }

        auto file_it = options.icons.find("file");
        if (file_it != options.icons.end()) {
            file_icon = file_it->second;
            options.icons.erase(file_it);
        }
        icons_by_extension = options.icons;
    }
    
    TreeCLI my_tree(options.max_recursion_depth, options.show_details, options.char_style, 
        options.tree_style, options.ignore_files, options.show_hyperlinks,
        absolute_current_path, options.ignore_patterns, options.details_format,
        icons_by_extension, file_icon, dir_icon);
    my_tree.display();

    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // std::cout << "Время выполнения: " << duration.count() << " миллисекунд" << std::endl;
    return 0;
}
