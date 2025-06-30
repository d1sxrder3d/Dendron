#include "./tree.h" 

#include <iostream>
#include <vector>
#include <string_view>
#include <algorithm>

namespace fs = std::filesystem;


TreeCLI::TreeCLI(short int max_recursion_depth, short int char_style, bool tree_style, bool ignore_files, bool show_hyperlinks, const fs::path& absolute_current_path, const std::vector<std::string>& ignore_patterns)
    : max_recursion_depth_(max_recursion_depth),
      char_style_(char_style),
      tree_style_(tree_style),
      ignore_files_(ignore_files),
      show_hyperlinks_(show_hyperlinks),
      absolute_current_path_(absolute_current_path),
      ignore_patterns_(ignore_patterns),
      absolute_current_path_str_(absolute_current_path.string()),
      br_to_obj_(std::string(TreeCLI::CHARS[0 + char_style * 4]) + TreeCLI::CHARS[3 + char_style * 4]),
      br_to_end_obj_(std::string(TreeCLI::CHARS[1 + char_style * 4]) + TreeCLI::CHARS[3 + char_style * 4]),
      br_(std::string(TreeCLI::CHARS[2 + char_style * 4]) + "   ") {}
    


void TreeCLI::display(const fs::path& directory_path) {
    if (fs::exists(directory_path)) {
        std::cout << directory_path.string() << std::endl; // Как вы и просили, отображаем пользовательский путь
        std::string prefix; // Начинаем с пустого префикса
        tree_recursive(this->absolute_current_path_, prefix, 0); // Но обход начинаем с канонического пути
    } else {
        std::cerr << "Error: Path does not exist: " << directory_path.string() << std::endl;
    }
}

template<typename Range>
bool TreeCLI::check_pattern(const Range& patterns, std::string_view filename) {
    for (const auto& pattern_item : patterns) {
        const std::string_view pattern_v(pattern_item);

        if (pattern_v.empty()) {
            continue;
        }

        const bool starts_with_star = pattern_v.front() == '*';
        const bool ends_with_star = pattern_v.length() > 1 && pattern_v.back() == '*';

        if (starts_with_star && ends_with_star) {

            const auto core_pattern = pattern_v.substr(1, pattern_v.length() - 2);

            if (filename.find(core_pattern) != std::string_view::npos) {
                return true;
            }
        } else if (starts_with_star) {

            const auto suffix = pattern_v.substr(1);

            if (filename.size() >= suffix.size() && 
                filename.substr(filename.size() - suffix.size()) == suffix) {
                return true;
            }
        } else if (ends_with_star) {

            const auto prefix = pattern_v.substr(0, pattern_v.length() - 1);

            if (filename.find(prefix) == 0) {
                return true;
            }
        } else {

            if (filename == pattern_v) {
                return true;
            }
        }
    }
    return false;
}

bool TreeCLI::should_ignore(std::string_view filename) const {
    return TreeCLI::check_pattern(ignore_patterns_, filename);
}

const char* TreeCLI::get_entry_color(const fs::directory_entry& entry) const {
    try {
        if (entry.is_symlink()) {
            return COLOR_SYMLINK;
        }
        if (entry.is_directory()) {
            return COLOR_DIR;
        }
        if (entry.is_regular_file()) {
            
            auto perms = entry.status().permissions();
            if ((perms & (fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec)) != fs::perms::none) {
                return COLOR_EXEC;
            }
            const auto filename_str = entry.path().filename().string();
            // Если не исполняемый, проверяем, не картинка ли это
            if (TreeCLI::check_pattern(TreeCLI::IMAGE_PATTERNS, filename_str)) {
                return COLOR_IMAGE;
            } else if (TreeCLI::check_pattern(TreeCLI::ARCHIVE_PATTERNS, filename_str)){
                return COLOR_ARCHIVE;
            }
        }
    } catch (const fs::filesystem_error&) {
        
    }
    return COLOR_REGULAR_FILE;
}


void TreeCLI::tree_recursive(const std::filesystem::path& directory_path, std::string& prefix, int current_depth) {
    
    if (max_recursion_depth_ != -1 && current_depth >= max_recursion_depth_) {
        return;
    }

    std::vector<fs::directory_entry> entries;
    try {
        for (const auto& entry : fs::directory_iterator(directory_path)) {
            if (should_ignore(entry.path().filename().string())) { // Выделение памяти здесь, к сожалению, трудно избежать для кроссплатформенности
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
            const bool a_is_dir = a.is_directory();
            const bool b_is_dir = b.is_directory();

            if (a_is_dir != b_is_dir) {
                
                return this->tree_style_ ? (a_is_dir < b_is_dir) : (a_is_dir > b_is_dir);
            }
            
            return a.path().filename() < b.path().filename();
        });
    
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        const bool is_last = (i == entries.size() - 1);
        
        std::stringstream line_ss;
        line_ss << prefix << (is_last ? br_to_end_obj_ : br_to_obj_) << " ";

        const char* color = get_entry_color(entry);

        if (this->show_hyperlinks_ && !entry.is_directory() && !entry.is_symlink()) {
            line_ss << color << "\e]8;;file://" << entry.path().string() 
                      << "\a" << entry.path().filename().string() << "\e]8;;\a";
        } else {
            line_ss << color << entry.path().filename().string();
        }


        if (entry.is_symlink()) {
            try {
                fs::path symlink_target = fs::read_symlink(entry.path());
                line_ss << " -> " << symlink_target.string();
            } catch (const fs::filesystem_error&) {
                line_ss << " -> [broken link]";
            }
        }

        line_ss << TreeCLI::COLOR_RESET;
        std::cout << line_ss.str() << std::endl;

        
        if (entry.is_directory() && !entry.is_symlink()) {
            // Модификация префикса "на месте": добавляем, рекурсируем, убираем
            const std::string& branch = is_last ? "    " : br_;
            prefix.append(branch);
            tree_recursive(entry.path(), prefix, current_depth + 1);
            prefix.resize(prefix.size() - branch.size());
        }
    }
}