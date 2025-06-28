#include "./tree.h" 

#include <iostream>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

TreeCLI::TreeCLI(int max_recursion_depth, int char_style, bool tree_style)
    : max_recursion_depth_(max_recursion_depth), char_style_(char_style), tree_style_(tree_style) {
}


void TreeCLI::display(const fs::path& directory_path) {
    if (fs::exists(directory_path)) {
        std::cout << directory_path.string() << std::endl;
        
        print_tree_recursive(directory_path, "", 0);
    } else {
        std::cerr << "Ошибка: Путь не существует: " << directory_path.string() << std::endl;
    }
}


void TreeCLI::print_tree_recursive(const fs::path& directory_path, const std::string& prefix, int current_depth) {
    
    if (max_recursion_depth_ != -1 && current_depth >= max_recursion_depth_) {
        return;
    }

    std::vector<fs::directory_entry> entries;
    try {
        for (const auto& entry : fs::directory_iterator(directory_path)) {
            entries.push_back(entry);
        }
    } catch (const fs::filesystem_error&) {
        
        return;
    }

    std::sort(entries.begin(), entries.end(),
        [this](const fs::directory_entry& a, const fs::directory_entry& b) {
            bool a_is_dir = a.is_directory();
            bool b_is_dir = b.is_directory();

            if (a_is_dir != b_is_dir) {
                if(!this->tree_style_)
                    return a_is_dir; 
                else
                    return a_is_dir < b_is_dir;
            }
            
            return a.path().filename() < b.path().filename();
        });

    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        const bool is_last = (i == entries.size() - 1);

        std::cout << prefix << (is_last ? "╚═" : "╠═") << " " << entry.path().filename().string() << std::endl;
        if (entry.is_directory()) {
            std::string next_prefix = prefix + (is_last ? "    " : "│   "); 
            print_tree_recursive(entry.path(), next_prefix, current_depth + 1);
        }
    }
}