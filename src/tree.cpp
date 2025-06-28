#include "./tree.h" 

#include <iostream>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

const char* const TreeCLI::chars_[12] = {
    "╠", "╚", "║", "═", // Style 0
    "┣", "┗", "┃", "━", // Style 1
    "├", "└", "│", "─"  // Style 2
};

TreeCLI::TreeCLI(int max_recursion_depth, int char_style, bool tree_style, bool ignore_files, const std::vector<std::string>& ignore_patterns)
    : br_to_obj_(std::string(chars_[0 + char_style * 4]) + chars_[3 + char_style * 4]),
      br_to_end_obj_(std::string(chars_[1 + char_style * 4]) + chars_[3 + char_style * 4]),
      br_(std::string(chars_[2 + char_style * 4]) + "   "),
      max_recursion_depth_(max_recursion_depth),
      char_style_(char_style), tree_style_(tree_style), ignore_files_(ignore_files),
      ignore_patterns_(ignore_patterns){
};
    


void TreeCLI::display(const fs::path& directory_path) {
    if (fs::exists(directory_path)) {
        std::cout << directory_path.string() << std::endl;
        
        print_tree_recursive(directory_path, "", 0);
    } else {
        std::cerr << "Ошибка: Путь не существует: " << directory_path.string() << std::endl;
    }
}

bool TreeCLI::should_ignore(const std::string& filename) const {
    for (const auto& pattern : ignore_patterns_) {
        if (pattern.empty()) continue;

        bool starts_with_star = pattern.front() == '*';
        bool ends_with_star = pattern.back() == '*';

        if (starts_with_star && ends_with_star && pattern.length() > 1) {
            
            if (filename.find(pattern.substr(1, pattern.length() - 2)) != std::string::npos) {
                return true;
            }
        } else if (starts_with_star) {
            
            std::string suffix = pattern.substr(1);
            if (filename.length() >= suffix.length() && filename.compare(filename.length() - suffix.length(), suffix.length(), suffix) == 0) {
                return true;
            }
        } else if (ends_with_star) {
            
            if (filename.rfind(pattern.substr(0, pattern.length() - 1), 0) == 0) {
                return true;
            }
        } else if (filename == pattern) { 
            return true;
        }
    }
    return false;
}

void TreeCLI::print_tree_recursive(const fs::path& directory_path, const std::string& prefix, int current_depth) {
    
    if (max_recursion_depth_ != -1 && current_depth >= max_recursion_depth_) {
        return;
    }

    std::vector<fs::directory_entry> entries;
    try {
        for (const auto& entry : fs::directory_iterator(directory_path)) {
            if (should_ignore(entry.path().filename().string())) {
                continue;
            }
            if (!ignore_files_ || entry.is_directory()) {
                entries.push_back(entry);
            }
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
                    // return b_is_dir;
                else
                    return a_is_dir < b_is_dir;
            }
            
            return a.path().filename() < b.path().filename();
        });
    
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        const bool is_last = (i == entries.size() - 1);

        std::cout << prefix << (is_last ? this->br_to_end_obj_: this->br_to_obj_) << " " << entry.path().filename().string() << std::endl;
        if (entry.is_directory()) {
            std::string next_prefix = prefix + (is_last ? "    " : this->br_); 
            print_tree_recursive(entry.path(), next_prefix, current_depth + 1);
        }
    }
}