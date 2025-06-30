#pragma once 

#include <filesystem>
#include <vector>
#include <map>
#include <string>



class TreeCLI{
public:
    explicit TreeCLI(short int max_recursion_depth, bool show_details, short int char_style, 
        bool tree_style, bool ignore_files, bool show_hyperlinks ,
        const std::filesystem::path& absolute_current_path, const std::vector<std::string>& ignore_patterns, 
        const std::string& details_format, const std::map<std::string, std::string>& icons_by_extension, 
        const std::string& default_file_icon, const std::string& directory_icon);

    /*
      @brief Отображает дерево директорий для указанного пути.
      @param directory_path Путь к директории для отображения.
     */
    void display();


private:
    template<typename Range>
    static bool check_pattern(const Range& patterns, std::string_view filename);

    bool should_ignore(std::string_view filename) const;
    const char* get_entry_color(const std::filesystem::directory_entry& entry, std::filesystem::file_status status) const;

    std::string get_icon(const std::filesystem::directory_entry& entry) const;

    void tree_recursive(const std::filesystem::path& path, std::string& prefix, int current_depth);


    static std::string format_permissions(std::filesystem::perms p, bool is_dir, bool is_symlink);
    static std::string format_size(uintmax_t size);
    static std::string format_time(std::filesystem::file_time_type ftime);
#if defined(__unix__) || defined(__APPLE__)
    static std::string format_owner(uid_t uid);
    static std::string format_group(gid_t gid);
#endif




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
    const short int max_recursion_depth_; 
    const bool show_details_;
    const short int char_style_;          // Стиль псевдографики
    const bool tree_style_;         
    const bool ignore_files_;
    const bool show_hyperlinks_;
    const std::filesystem::path& absolute_current_path_;
    const std::vector<std::string> ignore_patterns_;
    const std::string details_format_;

    // Derived/internal state
    const std::string absolute_current_path_str_;
    // ╠═ or ┣━ or ├─
    const std::string br_to_obj_;
    // ╚═ or ┗━ or └─
    const std::string br_to_end_obj_;
    // ║ or ┃ or │
    const std::string br_;

    const std::map<std::string, std::string> icons_by_extension_;
    const std::string default_file_icon_;
    const std::string directory_icon_ ;
};