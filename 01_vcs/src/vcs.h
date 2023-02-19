#ifndef VCS_H
#define VCS_H

#include <filesystem>
#include <string>
#include <unordered_map>

namespace vcs {

bool init();
bool snapshot(const std::string &message);
bool revert(const std::string &id);
bool log();
bool create_snapshot(std::filesystem::path &snapshot_dir);

} // namespace vcs

#endif