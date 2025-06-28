#pragma once 

#include <filesystem>
#include <string>


class TreeCLI{
public:
    explicit TreeCLI(int max_recursion_depth = -1, int char_style = 0, bool tree_style = false);

    /*
      @brief Отображает дерево директорий для указанного пути.
      @param directory_path Путь к директории для отображения.
     */
    void display(const std::filesystem::path& directory_path);


private:
    void print_tree_recursive(const std::filesystem::path& directory_path, const std::string& prefix, int current_depth);
    // Глубина рекурсии 
    // "-1" - отключено
    int max_recursion_depth_; 
    int char_style_; // Стиль дерева (в разработке)
    // 0 (default) - файлы ниже 
    // 1 - файлы выше
    bool tree_style_; 
};