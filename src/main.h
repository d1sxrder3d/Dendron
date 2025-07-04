#pragma once 

#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <string>


struct ProgramOptions {
    std::filesystem::path directory_path = "./"; 
    std::vector<std::string> ignore_patterns;
    short int max_recursion_depth = 1; // Default value: 1, safe mod
    short int char_style = 2;           // Default value: ╠═, ╚═, ║
    bool tree_style = false;      // Default value: directories first
    bool active_icons = true;     // Default value: show icons
    bool need_help = false;       // Default value: no help needed
    bool need_config = false;     // Default value: no config needed
    bool need_version = false;    // Default value: no version needed
    bool show_hyperlinks = true;  // Default value: show hl
    bool ignore_files = false;    // Default value: don't ignore files
    bool show_details = false;    // Default value: don't show details
    bool copy_to_clipboard = false; // Default value: don't copy to clipboard
    std::string details_format = "PST";
    std::map<std::string, std::string> icons;
    
};