#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "./tree.h"
#include "./main.h" 


namespace fs = std::filesystem;

std::string trim(const std::string& str) {
    const std::string whitespace = " \t";
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // пустая строка или строка из пробелов

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
    // Запасной вариант для разработки или переносимого режима
    return (fs::current_path() / "configs" / "config.ini").string();
}

//
void parse_config(ProgramOptions& options){
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
        } else if(key == "tree_style"){
            options.tree_style = (value == "true");
        } else if(key == "ignore_files"){
            options.ignore_files = (value == "true");
        }
    }
}

ProgramOptions set_options(int argc, char* argv[]) {

    ProgramOptions options; 
    
    parse_config(options);

    

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-d" || arg == "--directory") {
            if (i + 1 < argc) {
                options.directory_path = argv[++i];
            } else {
                std::cerr << "Error: for argument " << arg << " value required." << std::endl;
            }
        } else if (arg == "-r" || arg == "--recursion") {
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
        } else if (arg == "-t" || arg == "--tree") {
            options.tree_style = true;
        } else if (arg == "-f" || arg == "--files") { 
            options.ignore_files = true;
        } else if (arg == "-i" || arg == "--ignore") {
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                i++;
                options.ignore_patterns.push_back(argv[i]);
            }
        } else if (arg == "-h" || arg == "--help") {
            options.need_help = true;
            break; 
        } else {
            
            // Если аргумент не является флагом, считаем его путем.
            // Это позволяет пути, указанному в командной строке, переопределить путь из конфига.
            if (fs::exists(arg)) {
                options.directory_path = arg;
            } else {
                std::cerr << "Unknown argument or invalid path: " << arg << std::endl;
                options.need_help = true;
            }
        }
    }

    return options;
}




int main(int argc, char* argv[]) {

    ProgramOptions options = set_options(argc, argv);
    
    if (options.need_help) {
        std::cout << "Usage: ./tree_cli [path] [options]\n"
                  << "Options:\n"
                  << "  -d, --directory <path>     Specify the path to the directory\n"
                  << "  -r, --recursion <number>   Set maximum recursion depth\n"
                  << "  -s, --style <0-2>          Set tree style\n"
                  << "  -t, --tree                 Sort directories before files\n"
                  << "  -f, --files                Ignore files in output\n"
                  << "  -i, --ignore <pattern...>  Ignore files/directories using the pattern\n"
                  << "  -h, --help                 Show this message\n";
        return 0;
    }
    

    if (options.directory_path.empty()) {
        options.directory_path = fs::current_path();
    }
    
    TreeCLI my_tree(options.max_recursion_depth, options.char_style, options.tree_style, options.ignore_files, options.ignore_patterns);
    my_tree.display(options.directory_path);

    
    
    return 0;
}
