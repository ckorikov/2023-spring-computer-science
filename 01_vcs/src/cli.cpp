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
}