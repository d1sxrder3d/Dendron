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
                    i++; 
                    int depth = std::stoi(argv[i]);
                    if (depth >= 0) {
                        options.max_recursion_depth = depth;
                    } else {
                        throw std::exception();
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Ошибка: Неверное значение для глубины рекурсии. (безопасный режим, r = 1)" << std::endl;
                    options.max_recursion_depth = 1;
                }
            } else {
                std::cerr << "Ошибка: для аргумента " << arg << " требуется значение." << std::endl;
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
                    std::cerr << "Ошибка: Неверное значение для стиля. (безопасный режим, s = 0)" << std::endl;
                    options.char_style = 0;
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
                  << "  -s, --style <0-2>        Установить стиль оформления дерева\n"
                  << "  -t, --tree               Сортировать директории перед файлами\n"
                  << "  -f, --files              Игнорировать файлы при выводе\n"
                  << "  -i, --ignore <шаблон...> Игнорировать файлы/директории по шаблону\n"
                  << "  -h, --help               Показать это сообщение\n";
        return 0;
    }

    if (options.directory_path.empty()) {
        options.directory_path = fs::current_path();
    }
    
    TreeCLI my_tree(options.max_recursion_depth, options.char_style, options.tree_style, options.ignore_files, options.ignore_patterns);
    my_tree.display(options.directory_path);

    return 0;
}
