#include <iostream>
#include <string>

#include <cli.hpp>
#include <vcs.hpp>

bool handle_init_command(const std::vector<std::string> &args) {
  if (!args.empty()) {
    std::cerr << "Error: init command does not take any arguments."
              << std::endl;
    return false;
  }

  if (!vcs::init()) {
    std::cerr << "Error: repository already exists." << std::endl;
    return false;
  }

  std::cout << "Repository initialized successfully." << std::endl;
  return true;
}

bool handle_snapshot_command(const std::vector<std::string> &args) {
  if (args.empty()) {
    std::cerr << "Error: snapshot command requires a message argument."
              << std::endl;
    return false;
  }

  return vcs::snapshot(args[0]);
}

bool handle_diff_command(const std::vector<std::string> &args) {
  if (args.empty()) {
    std::cerr << "Error: revert command requires a snapshot id." << std::endl;
    return false;
  }

  return vcs::diff(args[0]);
}

bool handle_status_command(const std::vector<std::string> &args) {
  if (!args.empty()) {
    std::cerr << "Error: status command does not take any arguments."
              << std::endl;
    return false;
  }

  return vcs::status();
}
bool handle_revert_command(const std::vector<std::string> &args) {
  if (args.empty()) {
    std::cerr << "Error: revert command requires a snapshot id." << std::endl;
    return false;
  }

  return vcs::revert(args[0]);
}

bool handle_help_command(const std::vector<std::string> &args) {
  if (!args.empty()) {
    std::cerr << "Error: help command does not take any arguments."
              << std::endl;
    return false;
  }

  std::cout
      << "Usage: vcs <command> [<args>]\n\n"
         "Commands:\n"
         "  init      Create an empty vcs repository in the current directory\n"
         "  snapshot  Record changes to the repository\n"
         "  diff      Show differences between current state and last "
         "snapshot\n"
         "  log       Show the snapshot history\n"
         "  help      Show this message\n";

  return true;
}

bool handle_log_command(const std::vector<std::string> &args) {
  if (!args.empty()) {
    std::cerr << "Error: log command does not take any arguments." << std::endl;
    return false;
  }

  vcs::log();

  return true;
}