#include "./tree.h" 

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <string_view>
#include <algorithm>
#include <optional>
#include <iomanip>
#include <ctime>
#include <chrono>

#if defined(__unix__) || defined(__APPLE__)
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#endif

namespace fs = std::filesystem;



TreeCLI::TreeCLI(short int max_recursion_depth, bool show_details,short int char_style, 
    bool tree_style, bool ignore_files, 
    bool show_hyperlinks, const fs::path& absolute_current_path, 
    const std::vector<std::string>& ignore_patterns, const std::string& details_format,
    const std::map<std::string, std::string>& icons_by_extension, const std::string& default_file_icon, const std::string& directory_icon)
    : max_recursion_depth_(max_recursion_depth),
      show_details_(show_details),  
      char_style_(char_style),
      tree_style_(tree_style),
      ignore_files_(ignore_files),
      show_hyperlinks_(show_hyperlinks),
      absolute_current_path_(absolute_current_path),
      ignore_patterns_(ignore_patterns),
      details_format_(details_format),
      absolute_current_path_str_(absolute_current_path.string()),
      br_to_obj_(std::string(TreeCLI::CHARS[0 + char_style * 4]) + TreeCLI::CHARS[3 + char_style * 4]),
      br_to_end_obj_(std::string(TreeCLI::CHARS[1 + char_style * 4]) + TreeCLI::CHARS[3 + char_style * 4]),
      br_(std::string(TreeCLI::CHARS[2 + char_style * 4]) + "   "),
      icons_by_extension_(icons_by_extension), 
      default_file_icon_(default_file_icon),
      directory_icon_(directory_icon) {}


void TreeCLI::display() {
    if (fs::exists(absolute_current_path_)) {
        std::cout << absolute_current_path_.string() << std::endl;
        std::string prefix; 
        tree_recursive(absolute_current_path_, prefix, 0);
    } else {
        std::cerr << "Error: Path does not exist: " << absolute_current_path_.string() << std::endl;
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

const char* TreeCLI::get_entry_color(const fs::directory_entry& entry, fs::file_status status) const {

    try {
        if (fs::is_symlink(status)) {
            return COLOR_SYMLINK;
        }
        if (fs::is_directory(status)) {
            return COLOR_DIR;
        }
        if (fs::is_regular_file(status)) {
            auto perms = status.permissions();
            
            if ((perms & (fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec)) != fs::perms::none) {
                return COLOR_EXEC;
            }
            const auto filename_str = entry.path().filename().string();
           
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

std::string TreeCLI::get_icon(const fs::directory_entry& entry) const {
    if (entry.is_directory()) {
        if(entry.path().filename().string() == ".git"){
            auto it = icons_by_extension_.find("git");
                if (it != icons_by_extension_.end()) {
                    return it->second;
            }
        }
        return directory_icon_; 
    }

    const auto& path = entry.path();
    if (path.has_extension()) {
        std::string ext = path.extension().string();
        if (ext.length() > 1) { 

            auto it = icons_by_extension_.find(ext.substr(1)); 
            if (it != icons_by_extension_.end()) {
                return it->second;
            }
        }
    }

    return default_file_icon_; 
}

std::string TreeCLI::format_permissions(fs::perms p, bool is_dir, bool is_symlink) {
    std::string result;
    if (is_dir) result += 'd';
    else if (is_symlink) result += 'l';
    else result += '-';

    result += ((p & fs::perms::owner_read) != fs::perms::none ? 'r' : '-');
    result += ((p & fs::perms::owner_write) != fs::perms::none ? 'w' : '-');
    result += ((p & fs::perms::owner_exec) != fs::perms::none ? 'x' : '-');
    result += ((p & fs::perms::group_read) != fs::perms::none ? 'r' : '-');
    result += ((p & fs::perms::group_write) != fs::perms::none ? 'w' : '-');
    result += ((p & fs::perms::group_exec) != fs::perms::none ? 'x' : '-');
    result += ((p & fs::perms::others_read) != fs::perms::none ? 'r' : '-');
    result += ((p & fs::perms::others_write) != fs::perms::none ? 'w' : '-');
    result += ((p & fs::perms::others_exec) != fs::perms::none ? 'x' : '-');
    return result;
}

std::string TreeCLI::format_size(uintmax_t size) {
    std::stringstream ss;
    if (size < 1024) {
        ss << size;
    } else if (size < 1024 * 1024) {
        ss << std::fixed << std::setprecision(1) << (static_cast<double>(size) / 1024.0) << 'K';
    } else if (size < 1024 * 1024 * 1024) {
        ss << std::fixed << std::setprecision(1) << (static_cast<double>(size) / (1024.0 * 1024.0)) << 'M';
    } else {
        ss << std::fixed << std::setprecision(1) << (static_cast<double>(size) / (1024.0 * 1024.0 * 1024.0)) << 'G';
    }
    return ss.str();
}

std::string TreeCLI::format_time(fs::file_time_type ftime) {
    try {
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
        );
        std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

        // std::localtime не является потокобезопасным, но для этой утилиты это приемлемо.
        std::tm* tm = std::localtime(&cftime);
        if (!tm) {
            return "------------"; // 12 тире для выравнивания
        }

        // Проверяем, старше ли файл 6 месяцев, чтобы изменить формат даты (как в `ls -l`)
        auto now = std::chrono::system_clock::now();
        auto six_months_ago = now - std::chrono::hours(24 * 30 * 6);
        const char* fmt = (sctp < six_months_ago) ? "%b %e  %Y" : "%b %e %H:%M";

        std::stringstream ss;
        ss << std::put_time(tm, fmt);
        return ss.str();
    } catch (...) {
        return "------------";
    }
}

#if defined(__unix__) || defined(__APPLE__)
std::string TreeCLI::format_owner(uid_t uid) {
    if (struct passwd *pw = getpwuid(uid)) {
        return pw->pw_name;
    }
    return std::to_string(uid);
}

std::string TreeCLI::format_group(gid_t gid) {
    if (struct group *gr = getgrgid(gid)) {
        return gr->gr_name;
    }
    return std::to_string(gid);
}
#endif


void TreeCLI::tree_recursive(const std::filesystem::path& path, std::string& prefix, int current_depth) {
    
    if (max_recursion_depth_ != -1 && current_depth >= max_recursion_depth_) {
        return;
    }

    std::vector<fs::directory_entry> entries;
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
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

        

        fs::file_status status;
        try {
            status = entry.status();
        } catch (const fs::filesystem_error&) {
            
        }

        const char* color = get_entry_color(entry, status);
        line_ss << color;

        const std::string icon = get_icon(entry);
        if (!icon.empty()) {
            line_ss << icon << " ";
        }

        if (this->show_hyperlinks_ && fs::is_regular_file(status)) {
            line_ss << "\e]8;;file://" << entry.path().string() 
                      << "\a" << entry.path().filename().string() << "\e]8;;\a";
        } else {
            line_ss << entry.path().filename().string();
        }

        if (show_details_) {
            line_ss << " ";
            try {
                
                std::optional<fs::perms> perms;
                std::optional<uintmax_t> size;
                std::optional<fs::file_time_type> ftime;
                std::optional<struct stat> stat_buf;

                bool first_detail = true;
                for (char format_char : details_format_) {
                    if (!first_detail) line_ss << " "; 
                    first_detail = false;
                    switch(format_char) {
                        case 'P':
                            if (!perms) perms = status.permissions();
                            line_ss << format_permissions(*perms, fs::is_directory(status), fs::is_symlink(status));
                            break;
                        case 'S':
                            if (fs::is_regular_file(status)) {
                                if (!size) size = fs::file_size(entry);
                                line_ss << std::setw(6) << std::right << format_size(*size);
                            } else {
                                line_ss << std::setw(6) << std::right << "-";
                            }
                            break;
                        case 'T':
                            if (!ftime) ftime = fs::last_write_time(entry);
                            line_ss << format_time(*ftime);
                            break;
                        case 'L': case 'U': case 'G': {
                        
                            if (!stat_buf) {
                                struct stat s;
                                if (::stat(entry.path().c_str(), &s) == 0) {
                                    stat_buf = s;
                                }
                            }
                            if (stat_buf) {
                                if (format_char == 'L') {
                                    line_ss << std::setw(3) << std::right << stat_buf->st_nlink;
                                } else if (format_char == 'U') {
                                    line_ss << std::setw(8) << std::left << format_owner(stat_buf->st_uid);
                                } else { 
                                    line_ss << std::setw(8) << std::left << format_group(stat_buf->st_gid);
                                }
                            } else {
                                line_ss << "?";
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
            } catch (const fs::filesystem_error&) {
                line_ss << " [details error]";
            }
        }

        if (fs::is_symlink(status)) {
            try {
                fs::path symlink_target = fs::read_symlink(entry.path());
                line_ss << " -> " << symlink_target.string();
            } catch (const fs::filesystem_error&) {
                line_ss << " -> [broken link]";
            }
        }

        line_ss << TreeCLI::COLOR_RESET;
        std::cout << line_ss.str() << std::endl;

        
        if (fs::is_directory(status) && !fs::is_symlink(status)) {
            
            const std::string& branch = is_last ? "    " : br_;
            prefix.append(branch);
            tree_recursive(entry.path(), prefix, current_depth + 1);
            prefix.resize(prefix.size() - branch.size());
        }
    }
}