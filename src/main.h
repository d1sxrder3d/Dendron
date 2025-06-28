#pragma once 

#include <string>
#include <filesystem>

struct ProgramOptions {
    std::string directory_path = ""; 
    int max_recursion_depth = -1; // Значение по умолчанию: без ограничения глубины
    int char_style = 0;
    bool tree_style = false;      // Значение по умолчанию: директории сначала
    bool need_help = false;       // Значение по умолчанию: справка не нужна
};