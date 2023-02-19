#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include <utils.hpp>
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

/// @brief Create new snapshot
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
  std::cout << "Snapshot changes with message: " << message << std::endl;

  std::unordered_map<std::string, std::string> file_hashes;
  std::vector<std::regex> ignore_list;

  if (fs::is_regular_file("./.vcsignore")) {
    std::ifstream ignore_file("./.vcsignore");
    std::string str;
    while (std::getline(ignore_file, str)) {
      if (str.size() > 0)
        ignore_list.push_back(std::regex(str));
    }
  }

  for (const auto &entry : fs::recursive_directory_iterator(".")) {
    std::cout << entry.path().string() << std::endl;
    bool ignore = false;
    for (const auto &re : ignore_list) {
      if (std::regex_match(entry.path().string(), re))
        ignore = true;
    }

    if (ignore || (entry.path().string().rfind("./.archive", 0) == 0))
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

/// @brief Show filetree diff
/// @param id Snapshot id
/// @return true
bool diff(const std::string &id) {
  if (!fs::is_directory("./.archive/snapshots/" + id)) {
    std::cout << "No such snapshot" << std::endl;
    return false;
  }
  std::string snapshot_path = "./.archive/snapshots/" + id + "/files/";

  std::vector<std::string> snapshot_files, current_files, deleted_files,
      created_files, common_files;

  for (const auto &entry : fs::recursive_directory_iterator(snapshot_path)) {
    current_files.push_back(
        entry.path().string().substr(snapshot_path.length()));
  }

  for (const auto &entry : fs::recursive_directory_iterator(".")) {
    if (entry.path().string().rfind("./.archive", 0) == 0)
      continue;
    snapshot_files.push_back(entry.path().string().substr(2));
  }

  std::sort(snapshot_files.begin(), snapshot_files.end());
  std::sort(current_files.begin(), current_files.end());

  std::set_difference(snapshot_files.begin(), snapshot_files.end(),
                      current_files.begin(), current_files.end(),
                      std::back_inserter(created_files));

  std::set_difference(current_files.begin(), current_files.end(),
                      snapshot_files.begin(), snapshot_files.end(),
                      std::back_inserter(deleted_files));

  std::set_intersection(current_files.begin(), current_files.end(),
                        snapshot_files.begin(), snapshot_files.end(),
                        std::back_inserter(common_files));

  std::cout << "\t---\n";

  for (const auto &entry : deleted_files)
    std::cout << "Deleted " << entry << "\n\t---\n";

  for (const auto &entry : created_files)
    std::cout << "Created " << entry << "\n\t---\n";

  for (const auto &entry : common_files) {
    file_diff(entry, snapshot_path + entry);
    std::cout << "\t---\n";
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