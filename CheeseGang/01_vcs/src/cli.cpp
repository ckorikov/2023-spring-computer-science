#include <string>
#include <iostream>

#include "cli.h"
#include "vcs.h"

bool handle_init_command(const std::vector<std::string>& args) {
  if (!args.empty()) {
    std::cerr << "Error: init command does not take any arguments." << std::endl;
    return false;
  }

  if (!vcs_init()) {
    std::cerr << "Error: repository already exists." << std::endl;
    return false;
  }

  std::cout << "Repository initialized successfully." << std::endl;
  return true;
}

bool handle_commit_command(const std::vector<std::string>& args) {
  if (args.empty()) {
    std::cerr << "Error: commit command requires a message argument." << std::endl;
    return false;
  }

  return vcs_commit(args[0]);
}

bool handle_diff_command(const std::vector<std::string>& args) {
  if (!args.empty()) {
    std::cerr << "Error: diff command does not take any arguments." << std::endl;
    return false;
  }

  std::cout << "Displaying differences between current state and latest commit" << std::endl;

  return true;
}

bool handle_log_command(const std::vector<std::string>& args) {
  if (!args.empty()) {
    std::cerr << "Error: log command does not take any arguments." << std::endl;
    return false;
  }

  vcs_log();

  return true;
 
bool handle_help_command(const std::vector<std::string>&args) {
    if (!args.empty()) {
        std::cerr << "Error: help command does not take any arguments." << std::endl;
        return false;
    }

    vcs_help();

    return true;
}

bool handle_listhistoryforfile_command(const fs::path& filename) {
    std::vector<std::pair<int, std::unordered_map<std::string, std::string>>> snapshots;

    for (const auto& entry : fs::recursive_directory_iterator(".archive")) {
        if (fs::is_directory(entry.path())) {
            int snapshot_id = std::stoi(entry.path().filename().string().substr(10));
            std::string hashes_file_path = (entry.path() / "hashes.log").string();
            std::ifstream hashes_log_file(hashes_file_path);
            std::unordered_map<std::string, std::string> snapshot_hashes;
            for (std::string line; getline(hashes_log_file, line); ) {
                std::istringstream iss(line);
                std::string file;
                std::string hash;
                iss >> file >> hash;
                snapshot_hashes[file] = hash;
            }
            snapshots.emplace_back(snapshot_id, snapshot_hashes);
        }
    }
    std::sort(snapshots.begin(), snapshots.end(), [](auto a, auto b) { return a.first > b.first;});

    for (const auto& [snapshot_id, snapshot_hashes] : snapshots) {
        std::string expected_hash = snapshot_hashes[filename.string().substr(2)];
        std::string actual_hash = compute_file_hash(filename);
        if (expected_hash == actual_hash) {
            std::cout << "File was not modified since snapshot " << snapshot_id << std::endl;
        }
        else {
            std::cout << "File was modified after snapshot " << snapshot_id << std::endl;
        }
    
        return true;
    }