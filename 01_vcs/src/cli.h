#ifndef CLI_H
#define CLI_H

#include <vector>
#include <string>

bool handle_init_command(const std::vector<std::string>& args);
bool handle_commit_command(const std::vector<std::string>& args);
bool handle_diff_command(const std::vector<std::string>& args);
bool handle_log_command(const std::vector<std::string>& args);

#endif