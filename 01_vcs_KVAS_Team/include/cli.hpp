#ifndef CLI_HPP
#define CLI_HPP

#include <string>
#include <vector>

bool handle_init_command(const std::vector<std::string> &args);
bool handle_snapshot_command(const std::vector<std::string> &args);
bool handle_diff_command(const std::vector<std::string> &args);
bool handle_status_command(const std::vector<std::string> &args);
bool handle_log_command(const std::vector<std::string> &args);
bool handle_revert_command(const std::vector<std::string> &args);
bool handle_help_command(const std::vector<std::string> &args);

#endif