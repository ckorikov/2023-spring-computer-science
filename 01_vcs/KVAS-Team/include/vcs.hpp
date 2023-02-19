#ifndef VCS_HPP
#define VCS_HPP

#include <string>
#include <unordered_map>

namespace vcs {

bool init();
bool snapshot(const std::string &message);
bool revert(const std::string &id);
bool diff(const std::string &id);
bool log();

} // namespace vcs

#endif