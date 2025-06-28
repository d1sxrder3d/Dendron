#pragma once 

#include <string>
#include <filesystem>
#include <vector>
#include <string>



struct ProgramOptions {
    std::string directory_path = ""; 
    std::vector<std::string> ignore_patterns;
    int max_recursion_depth = -1; // Значение по умолчанию: без ограничения глубины
    int char_style = 2;           // Значение по умолчанию: ╠═, ╚═, ║
    bool tree_style = false;      // Значение по умолчанию: директории сначала
    bool need_help = false;       // Значение по умолчанию: справка не нужна
    bool ignore_files = false;    // Значение по умолчанию: не игнорировать файлы
 
};