#include <string>
#include <iostream>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <dirent.h>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>

#include "vcs.h"

namespace fs = std::filesystem;

bool vcs_init() {
  fs::path archive_dir = ".archive";
  if (fs::exists(archive_dir) && fs::is_directory(archive_dir)) {
    return false;
  }

  fs::create_directory(archive_dir);
  std::cout << "Created directory " << archive_dir << std::endl;

  create_snapshot();

  return true;
}


bool vcs_commit(const std::string& message) {
  // Code to update the version control repository with the changes

  // For this example, we'll just output a message
  std::cout << "Committing changes with message: " << message << std::endl;

  return true;
}

int get_next_snapshot_id() {
  int snapshot_id = 0;
  std::string snapshot_dir = ".archive/snapshot_" + std::to_string(snapshot_id);
  while (fs::exists(snapshot_dir)) {
    snapshot_id++;
    snapshot_dir = ".archive/snapshot_" + std::to_string(snapshot_id);
  }
  return snapshot_id;
}

void create_snapshot_directory(int snapshot_id) {
  std::string snapshot_dir = ".archive/snapshot_" + std::to_string(snapshot_id);
  fs::create_directory(snapshot_dir);
}

void copy_from_to(const fs::path &src_path, const fs::path &dst_path) {
  std::ifstream src(src_path, std::ios::binary);
  std::ofstream dst(dst_path, std::ios::binary);
  dst << src.rdbuf();
}

std::string compute_file_hash(const fs::path &path) {
  std::ifstream src(path, std::ios::binary);
  src.clear();
  src.seekg(0, std::ios::beg);
  std::istreambuf_iterator<char> src_begin(src);
  std::istreambuf_iterator<char> src_end;
  std::string data(src_begin, src_end);
  std::hash<std::string> hash_fn;
  std::size_t hash_value = hash_fn(data);

  std::stringstream ss;
  ss << std::hex << std::setfill('0') << std::setw(sizeof(std::size_t) * 2) << hash_value;
  return ss.str();
}

std::unordered_map<std::string, std::string> create_snapshot() {
  int snapshot_id = get_next_snapshot_id();
  create_snapshot_directory(snapshot_id);
  std::string snapshot_dir = ".archive/snapshot_" + std::to_string(snapshot_id);

  std::unordered_map<std::string, std::string> file_hashes;

  for (const auto &entry : fs::recursive_directory_iterator(".")) {
    if (!fs::is_directory(entry.path())) {
      copy_from_to(entry.path(), snapshot_dir / entry.path().filename());
      file_hashes[entry.path().string().substr(2)] = compute_file_hash(entry.path());
    }
  }

  auto hashes_log_file_path = fs::path(snapshot_dir) / std::string("hashes.log");
  std::ofstream hashes_log_file(hashes_log_file_path);
  for (const auto &[file, hash] : file_hashes) {
    hashes_log_file << file << ' ' << hash << '\n';
  }
  hashes_log_file.close();

  return file_hashes;
}

bool vcs_log() {
  std::cout << "Log message" << std::endl;

  return true;
}
