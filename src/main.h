#pragma once 

#include <string>
#include <filesystem>
#include <vector>
#include <string>


struct ProgramOptions {
    std::filesystem::path directory_path = "./"; 
    std::vector<std::string> ignore_patterns;
    int max_recursion_depth = 1; // Default value: 1, safe mod
    int char_style = 2;           // Default value: ╠═, ╚═, ║
    bool tree_style = false;      // Default value: directories first
    bool need_help = false;       // Default value: no help needed
    bool ignore_files = false;    // Default value: don't ignore files
 
};