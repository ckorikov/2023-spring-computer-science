#include "vcs.h"
#include "cli.h"


bool check_args(const std::string& value, const std::vector<std::string>& array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

int main(int argc, char* argv[])
{
    std::vector<std::string> commands{ "status", "commit", "push", "remove", "log", "--help", "checkout", "branch", "merge" };
    create_table();
    if (!fs::exists(".revisions")) {
        create_revisions_directory(".revisions");
        std::ofstream head(".revisions/HEAD");
        head << "main";
        head.close();
        create_revisions_directory(".revisions/branches/");
        std::ofstream branches(".revisions/branches/branches.txt");
        branches << "main\n";
        branches.close();
    }

    if (argc == 1) {
        cout << "Incorrect usage. Run ./a.out <add/commit/push>. Run ./main.out --help for more information." << endl;
        return 1;
    }

    if (check_args(argv[1], commands)) {
        if (strcmp(argv[1], "commit") == 0) {
            if (argc != 3) {
                cout << "Invalid format: Enter ./main.out commit <message> " << endl;
                return -1;
            }
            commit(argv[2]);
        }
<<<<<<< HEAD
        if (strcmp(argv[1], "status") == 0)
            status();
        if (strcmp(argv[1], "log") == 0)
            show_log();
        if (strcmp(argv[1], "push") == 0)
            push_to_server();
        if (strcmp(argv[1], "--help") == 0)
            display_help();
        if (strcmp(argv[1], "checkout") == 0) {
            if (!(2 < argc < 4)) {
                cout << "Invalid format: Enter ./main.out checkout <commit_id> or ./main.out checkout -b <branch_name>" << endl;
                return -1;
            }
            if (strcmp(argv[2], "-b") != 0)
                checkout_commit_id(argv[2]);
            else {
                create_branch_and_checkout(argv[3]);
            }
        }
        if ((strcmp(argv[1], "branch") == 0)) {
            display_branches();
        }
        if (strcmp(argv[1], "merge") == 0) {
            if (argc != 4)
                cout << "Invalid format: Enter ./main.out merge <branch2> <branch1>" << endl;
            else {
                merge(argv[2], argv[3]);
            }
        }
    }
    else {
        cout << "Unknown command: " << argv[1] << endl << "Run ./a.out <add/commit/push>" << endl;
    }
    return 0;
}
=======
        else
        {
            logic_ref.variables.clear(); // added to clear existing variables
            std::string result = logic_ref.process_math();
            return hbox(text(result));
        }
    }

    bool UI::process_events(Event event)
    {
        if (event == Event::Escape)
        {
            screen.ExitLoopClosure()();
            return true;
        }
        else if (event == Event::Return)
        {
            element_output = render_output();
            logic_ref.expression.clear();
            return true;
        }
        else if (event == Event::Custom)
        {
            // added for variable support
            std::string input = logic_ref.expression;

            // check if it is a variable declaration
            std::size_t pos = input.find("=");
            if (pos != std::string::npos)
            {
                std::string var_name = input.substr(0, pos);
                std::string var_value_str = input.substr(pos + 1);
                try
                {
                    int var_value = std::stoi(var_value_str);
                    logic_ref.variables[var_name] = var_value;
                    element_output = hbox(text("Variable created: "), text(var_name), text(" = "), text(var_value_str));
                    return true;
                }
                catch (const std::invalid_argument &ia)
                {
                    element_output = hbox(text("Error: Invalid argument"));
                    return true;
                }
            }
        }
        return false;
    }

    void UI::start()
    {
        auto component = Container::Vertical({expression_input_box});

        auto renderer = Renderer(component, [&]
                                 { return vbox({render_input(), separator(), element_output}) | border; });

        renderer |= CatchEvent([&](Event event)
                               { return process_events(event); });

        screen.Loop(renderer);
    }

} // namespace calc
>>>>>>> refs/remotes/origin/main
