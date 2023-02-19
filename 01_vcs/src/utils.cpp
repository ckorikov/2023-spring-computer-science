#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

/// @brief Get file hash
/// @param path File path
/// @return Hash string
std::string get_file_hash(const fs::path &path) {
  std::ifstream src(path, std::ios::binary);
  src.clear();
  src.seekg(0, std::ios::beg);
  std::istreambuf_iterator<char> src_begin(src);
  std::istreambuf_iterator<char> src_end;
  std::string data(src_begin, src_end);
  std::hash<std::string> hash_fn;
  std::size_t hash_value = hash_fn(data);

  std::stringstream ss;
  ss << std::hex << std::setfill('0') << std::setw(sizeof(std::size_t) * 2)
     << hash_value;
  return ss.str();
}

/// @brief File copy wrapper
/// @param src_path Source file
/// @param dst_path Dest file
void copy_from_to(const fs::path &src_path, const fs::path &dst_path) {
  std::ifstream src(src_path, std::ios::binary);
  std::ofstream dst(dst_path, std::ios::binary);
  dst << src.rdbuf();
}

/// @brief Print diff between two files
/// @param file1 first
/// @param file2 second
void file_diff(const fs::path &file1, const fs::path &file2) {
  std::ifstream file1_stream(file1);
  std::ifstream file2_stream(file2);

  std::string line1, line2;
  int line_num = 1;

  while (std::getline(file1_stream, line1) &&
         std::getline(file2_stream, line2)) {
    if (line1 != line2) {
      std::cout << "Line " << line_num << " differs:\n"
                << "< " << line1 << '\n'
                << "> " << line2 << '\n';
    }
    ++line_num;
  }

  while (std::getline(file1_stream, line1)) {
    std::cout << "Line " << line_num << " in " << file1 << " only:\n"
              << "< " << line1 << '\n';
    ++line_num;
  }

  while (std::getline(file2_stream, line2)) {
    std::cout << "Line " << line_num << " in " << file2 << " only:\n"
              << "> " << line2 << '\n';
    ++line_num;
  }
}

/// @brief Get next snapthot id (0, 1...)
/// @return int snapthot id
int get_next_snapshot_id() {
  int snapshot_id = 0;
  std::string snapshot_dir =
      ".archive/snapshots/" + std::to_string(snapshot_id);
  while (fs::exists(snapshot_dir)) {
    snapshot_id++;
    snapshot_dir = ".archive/snapshots/" + std::to_string(snapshot_id);
  }
  return snapshot_id;
}