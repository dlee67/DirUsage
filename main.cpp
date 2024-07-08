#include <iostream>
#include <filesystem>
#include <sys/statvfs.h>

namespace fs = std::filesystem;

uintmax_t calculate_directory_size(const fs::path& dir_path) {
    uintmax_t size = 0;
    for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {
        if (fs::is_regular_file(entry.status())) {
            size += fs::file_size(entry);
        }
    }
    return size;
}

bool get_filesystem_stats(const fs::path& path, struct statvfs& stats) {
    return (statvfs(path.c_str(), &stats) == 0);
}

int main() {
    fs::path dir_path;
    std::cout << "Enter the directory path: ";
    std::cin >> dir_path;

    if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
        std::cerr << "The specified path does not exist or is not a directory." << std::endl;
        return 1;
    }

    uintmax_t dir_size = calculate_directory_size(dir_path);
    struct statvfs stats;

    if (!get_filesystem_stats(dir_path, stats)) {
        std::cerr << "Failed to get filesystem statistics." << std::endl;
        return 1;
    }

    uintmax_t total_space = stats.f_blocks * stats.f_frsize;
    uintmax_t available_space = stats.f_bavail * stats.f_frsize;

    std::cout << "Directory size: " << dir_size << " bytes" << std::endl;
    std::cout << "Total storage: " << total_space << " bytes" << std::endl;
    std::cout << "Available storage: " << available_space << " bytes" << std::endl;

    double usage_ratio = static_cast<double>(dir_size) / available_space * 100;
    std::cout << "Directory usage compared to available storage: " << usage_ratio << "%" << std::endl;

    return 0;
}
