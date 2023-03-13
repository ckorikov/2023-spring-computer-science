#ifndef VCS_H
#define VCS_H

#include <string>
#include <unordered_map>

bool vcs_init();
bool vcs_commit(const std::string& message);
bool vcs_log();
bool vcs_help();

std::unordered_map<std::string, std::string> create_snapshot();

#endif