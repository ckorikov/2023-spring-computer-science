#ifndef CLI_H
#define CLI_H

#include <vector>
#include <string>

bool handle_init_command(const std::vector<std::string>& args);
bool handle_commit_command(const std::vector<std::string>& args);
bool handle_diff_command(const std::vector<std::string>& args);
bool handle_log_command(const std::vector<std::string>& args);
bool get_repository_path(const std::vector<std::string>& args);
bool filter_diff_callback(const std::vector<std::string>& args);
bool print_diff_callback(const std::vector<std::string>& args);
bool handle_label_command(const std::vector<std::string>& args);
bool handle_help_command(const std::vector<std::string>& args);
bool handle_listhistoryforfile_command(const fs::path& filename)



#endif