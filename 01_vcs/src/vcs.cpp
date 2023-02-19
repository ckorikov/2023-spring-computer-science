#include <algorithm>
#include <chrono>
#include <cstring>
#include <dirent.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include <vcs.hpp>

namespace vcs {

namespace fs = std::filesystem;

/// @brief Init repo, create aux dirs
/// @return false if already initialised otherwise true
bool init() {
  fs::path archive_dir = ".archive/snapshots";

  if (fs::exists(archive_dir) && fs::is_directory(archive_dir))
    return false;

  fs::create_directories(archive_dir);

  std::cout << "Initialised successfuly" << archive_dir << std::endl;

  return true;
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

/// @brief `create_snapshot()` wrapper
/// @param message Snapshot info message
/// @return False if no changes or if not in repo, otherwise true
bool snapshot(const std::string &message) {
  fs::path archive_dir = ".archive/snapshots";
  if (!fs::is_directory(archive_dir)) {
    std::cout << "Not in repo" << std::endl;
    return false;
  }
  int snapshot_id = get_next_snapshot_id();
  fs::path snapshot_dir = ".archive/snapshots/" + std::to_string(snapshot_id);
  fs::create_directories(snapshot_dir / "files");

  std::ofstream snapshot_info_file(snapshot_dir / "info");
  snapshot_info_file << snapshot_id << ": " << message;
  // std::cout << "Snapshotting changes with message: " << message << std::endl;

  return create_snapshot(snapshot_dir);
}

/// @brief File copy wrapper
/// @param src_path Source file
/// @param dst_path Dest file
void copy_from_to(const fs::path &src_path, const fs::path &dst_path) {
  std::ifstream src(src_path, std::ios::binary);
  std::ofstream dst(dst_path, std::ios::binary);
  dst << src.rdbuf();
}

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

/// @brief
/// @return
bool create_snapshot(fs::path &snapshot_dir) {
  std::unordered_map<std::string, std::string> file_hashes;

  for (const auto &entry : fs::recursive_directory_iterator(".")) {
    if (entry.path().string().rfind("./.archive", 0) == 0)
      continue;

    if (fs::is_directory(entry.path())) {
      fs::create_directories(snapshot_dir / "files" / entry.path());
    } else {
      copy_from_to(entry.path(), snapshot_dir / "files" / entry.path());
      file_hashes[entry.path().string()] = get_file_hash(entry.path());
    }
  }

  std::ofstream hashes_log_file(snapshot_dir / "hashes.log");
  for (const auto &[file, hash] : file_hashes)
    hashes_log_file << file << ' ' << hash << '\n';

  return true;
}

/// @brief Revert filetree to snapshot state
/// @param id snapshot id
/// @return False if snapshot does not exist
bool revert(const std::string &id) {
  if (!fs::is_directory("./.archive/snapshots/" + id)) {
    std::cout << "No such snapshot" << std::endl;
    return false;
  }

  std::string path = "./.archive/snapshots/" + id + "/files/";

  for (const auto &entry : fs::recursive_directory_iterator(path)) {

    if (fs::is_directory(entry.path())) {
      fs::create_directories(entry.path());
    } else {
      copy_from_to(entry.path(),
                   "./" + entry.path().string().substr(path.length()));
    }
  }

  return true;
}

/// @brief And you dont seem to understand
/// @return A shame you seemed an honest man
bool log() {
  std::vector<fs::directory_entry> dirs;
  for (const auto &entry : fs::directory_iterator("./.archive/snapshots"))
    dirs.push_back(entry);

  for (auto it = dirs.rbegin(); it != dirs.rend(); it++) {
    std::ifstream info_file(it->path() / "info");
    std::string info((std::istreambuf_iterator<char>(info_file)),
                     std::istreambuf_iterator<char>());
    std::cout << info << '\n';
  }
  return true;
}

} // namespace vcs