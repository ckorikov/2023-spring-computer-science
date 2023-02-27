#include <iostream>
#include <string>
#include <vector>

#include <cli.hpp>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Error: missing command" << std::endl;
    return 1;
  }

  std::string command = argv[1];
  std::vector<std::string> args(argv + 2, argv + argc);

  if (command == "init") {
    handle_init_command(args);
  } else if (command == "snapshot") {
    handle_snapshot_command(args);
  } else if (command == "diff") {
    handle_diff_command(args);
  } else if (command == "status") {
    handle_status_command(args);
  } else if (command == "log") {
    handle_log_command(args);
  } else if (command == "revert") {
    handle_revert_command(args);
  } else if (command == "-h") {
    handle_help_command(args);
  } else {
    std::cerr << "Error: unknown command: " << command << std::endl;
    return 1;
  }

  return 0;
}