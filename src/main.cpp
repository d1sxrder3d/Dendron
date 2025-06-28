#include <iostream>
#include <filesystem>
#include "./tree.h"
#include "./main.h" 

namespace fs = std::filesystem;

ProgramOptions parse_args(int argc, char* argv[]) {
    ProgramOptions options; 

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-d" || arg == "--directory") {
            if (i + 1 < argc) {
                options.directory_path = argv[++i];
            } else {
                std::cerr << "Ошибка: для аргумента " << arg << " требуется значение." << std::endl;
            }
        } else if (arg == "-r" || arg == "--recursion") {
            if (i + 1 < argc) {
                try {
                    options.max_recursion_depth = std::stoi(argv[++i]);
                } catch (const std::exception& e) {
                    std::cerr << "Ошибка: неверное значение для глубины рекурсии." << std::endl;
                }
            }
        } else if (arg == "-t" || arg == "--tree") {
            options.tree_style = true;
        } else if (arg == "-h" || arg == "--help") {
            options.need_help = true;
            break; 
        } else {
            
            if (options.directory_path.empty() && fs::exists(arg)) {
                options.directory_path = arg;
            } else {
                std::cerr << "Неизвестный аргумент: " << arg << std::endl;
                options.need_help = true;
            }
        }
    }

    return options;
}

int main(int argc, char* argv[]) {
    ProgramOptions options = parse_args(argc, argv);

    if (options.need_help) {
        std::cout << "Использование: ./tree_cli [путь] [опции]\n"
                  << "Опции:\n"
                  << "  -d, --directory <путь>   Указать путь к директории\n"
                  << "  -r, --recursion <число>  Установить максимальную глубину рекурсии\n"
                  << "  -t, --tree               Сортировать файлы перед директориями\n"
                  << "  -h, --help               Показать это сообщение\n";
        return 0;
    }

    if (options.directory_path.empty()) {
        options.directory_path = fs::current_path();
    }
    
    TreeCLI my_tree(options.max_recursion_depth, options.char_style, options.tree_style);
    my_tree.display(options.directory_path);

    return 0;
}
