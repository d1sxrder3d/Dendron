#pragma once
#include <string>
#include <vector>
#include <map>

struct ProgramOptions {
    std::string directory_path;
    int max_recursion_depth = -1;
    int char_style = 2;
    bool tree_style = false;
    bool ignore_files = false;
    bool show_hyperlinks = true;
    bool copy_to_clipboard = false;
    bool active_icons = true;
    bool show_details = false;
    std::string details_format = "P,S,T";
    std::vector<std::string> ignore_patterns;
    std::map<std::string, std::string> icons;

    bool need_help = false;
    bool need_version = false;
    bool need_config = false;
    bool generate_json = false;
    std::string json_output_path = "dendron_tree.json";
};