#ifndef UTILS_HPP
#define UTILS_HPP

#include <filesystem>

namespace fs = std::filesystem;

std::string get_file_hash(const fs::path &path);
void copy_from_to(const fs::path &src_path, const fs::path &dst_path);
void file_diff(const fs::path &file1, const fs::path &file2);
int get_next_snapshot_id();

#endif