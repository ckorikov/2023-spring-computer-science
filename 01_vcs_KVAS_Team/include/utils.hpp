#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

std::string get_file_hash(const fs::path &path);
void copy_from_to(const fs::path &src_path, const fs::path &dst_path);
void file_diff(const fs::path &file1, const fs::path &file2);
int get_curr_snapshot_id();

template <typename T>
std::vector<T> vector_diff(std::vector<T> v1, std::vector<T> v2) {
  if (v1.empty())
    return v2;
  if (v2.empty())
    return v1;

  std::vector<T> v3;
  std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
                      std::back_inserter(v3));
  return v3;
}

template <typename T>
inline std::vector<T> vector_inter(std::vector<T> v1, std::vector<T> v2) {
  if (v1.empty() || v2.empty())
    return std::vector<T>{};
  std::vector<T> v3;
  std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        std::back_inserter(v3));
  return v3;
}

#endif