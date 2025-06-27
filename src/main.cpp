#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;



std::string sub_path_cleaning(int path_length, const std::string& sub_path) {
    if (path_length < 0) {
        return sub_path; 
    }

    size_t start_pos = static_cast<size_t>(path_length) + 1;

    if (start_pos > sub_path.size()) {
        throw std::out_of_range("Invalid path_length");
    }

    return sub_path.substr(start_pos);
}

bool is_directory_empty_exceptions(const fs::path& dir_path) {
    if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
        return false;
    }
    
    fs::directory_iterator iter(dir_path);
    return iter == fs::directory_iterator{};
}

void tree(const int& current_path_lenght, const fs::path& now_dir){
    
    if(is_directory_empty_exceptions(now_dir)){
        return;
    }
    else{
        for(const auto& entry : fs::directory_iterator(now_dir)){

            std::string sub_path_clean = sub_path_cleaning(current_path_lenght, entry.path().string());

            if (entry.is_directory()) {
                std::cout << "dir: ";
                std::cout << sub_path_clean << "\n";

                tree(current_path_lenght + sub_path_clean.length() + 1, entry.path());

            }

            else if (entry.is_regular_file()) {
                std::cout << "file: ";

                std::cout << sub_path_clean << "\n";
                

            }

        
        }
    }

}


int main(){

    fs::path current_dir = fs::current_path();
    
    int current_path_lenght = current_dir.string().length();
    
    std::cout << "Текущая дирректория: " << current_dir << "\n";
    
    tree(current_path_lenght, current_dir);
    

    
    return 0;
}

