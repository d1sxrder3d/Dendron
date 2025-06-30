#pragma once 

#include <filesystem>
#include <vector>
#include <string>



class TreeCLI{
public:
    explicit TreeCLI(int max_recursion_depth, int char_style, bool tree_style, bool ignore_files, bool show_hyperlinks ,const std::filesystem::path& absolute_current_path, const std::vector<std::string>& ignore_patterns);

    /*
      @brief Отображает дерево директорий для указанного пути.
      @param directory_path Путь к директории для отображения.
     */
    void display(const std::filesystem::path& directory_path);


private:
    template<typename Range>
    static bool check_pattern(const Range& patterns, const std::string& filename);

    bool should_ignore(const std::string& filename) const;
    const char* get_entry_color(const std::filesystem::directory_entry& entry) const;


    void tree_recursive(const std::filesystem::path& directory_path, const std::string& prefix, int current_depth);


    void print_object(const std::filesystem::directory_entry& entry);

    //Массив стилей псевдографики
    static constexpr const char* const CHARS[12] = {
        "╠", "╚", "║", "═", // Style 0
        "┣", "┗", "┃", "━", // Style 1
        "├", "└", "│", "─"  // Style 2
    };
    //Массив паттернов расширений изображений
    static constexpr const char* const IMAGE_PATTERNS[] = {
        "*.jpg", "*.png", "*.gif", 
        "*.bmp","*.webp", "*.jpeg", 
        "*.raw", "*.svg", "*.ico", 
        "*.tiff"
    };
    //Массив паттернов расширений архивов
    static constexpr const char* const ARCHIVE_PATTERNS[] = {
        "*.zip", "*.rar", "*.7z", 
        "*.tar", "*.gz", "*.bz2", 
        "*.xz", "*.tar.gz", "*.tar.bz2", 
        "*.tar.xz"
    };


    // Цвета ANSI
    static constexpr const char* const COLOR_RESET = "\033[0m";
    static constexpr const char* const COLOR_REGULAR_FILE = "\033[37m"; // Белый
    static constexpr const char* const COLOR_IMAGE = "\033[33m";  // Желтый
    static constexpr const char* const COLOR_EXEC = "\033[32m"; // Зеленый
    static constexpr const char* const COLOR_DIR = "\033[34m";  // Синий
    static constexpr const char* const COLOR_SYMLINK = "\033[36m"; // Голубой
    static constexpr const char* const COLOR_ARCHIVE = "\033[35m"; // Фиолетовый
    
    // --- Member Variables ---

    // Options from CLI/config
    const int max_recursion_depth_; 
    const int char_style_;          // Стиль псевдографики
    const bool tree_style_;         
    const bool ignore_files_;
    const bool show_hyperlinks_;
    const std::filesystem::path& absolute_current_path_;
    const std::vector<std::string> ignore_patterns_;

    // Derived/internal state
    const std::string absolute_current_path_str_;
    // ╠═ or ┣━ or ├─
    const std::string br_to_obj_;
    // ╚═ or ┗━ or └─
    const std::string br_to_end_obj_;
    // ║ or ┃ or │
    const std::string br_;
};