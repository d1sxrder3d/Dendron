#pragma once 

#include <filesystem>
#include <vector>
#include <string>


class TreeCLI{
public:
    explicit TreeCLI(int max_recursion_depth, int char_style, bool tree_style, bool ignore_files, const std::vector<std::string>& ignore_patterns);

    /*
      @brief Отображает дерево директорий для указанного пути.
      @param directory_path Путь к директории для отображения.
     */
    void display(const std::filesystem::path& directory_path);


private:
    bool should_ignore(const std::string& filename) const;
    void print_tree_recursive(const std::filesystem::path& directory_path, const std::string& prefix, int current_depth);
    
    const int char_style_; // Стиль дерева (в разработке)
    
    //стили дерева
    static const char* const chars_[12];


    // ╠═ or ┣━ or ├─
    const std::string br_to_obj_;
    
    // ╚═ or ┗━ or └─
    const std::string br_to_end_obj_;
    
    // ║ or ┃ or │
    const std::string br_;
 

    // Глубина рекурсии 
    // "-1" - отключено
    const int max_recursion_depth_; 
    // 0 (default) - файлы ниже 
    // 1 - файлы выше
    const bool tree_style_; 
    const bool ignore_files_;
    const std::vector<std::string> ignore_patterns_;

};