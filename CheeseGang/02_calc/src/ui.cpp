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
#include "ui.h"

namespace calc
{

    std::vector<int> demo_triangle(int width, int height)
    {
        std::vector<int> output(width);
        for (int i = 0; i < width; ++i)
        {
            output[i] = i % (height - 4) + 2;
        }
        return output;
    }

    Element UI::render_input()
    {
        return hbox(text(" Expression: "), expression_input_box->Render());
    }

    Element UI::render_output()
    {
        if (logic_ref.expression == "plot")
        {
            return graph(std::ref(demo_triangle)) | color(Color::BlueLight);
        }
        else
        {
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
    while (running) {
        auto event = screen.PollEvent();
        running = process_events(event);

        if (logic_ref.expression_changed()) {
            logic_ref.evaluate_expression();
            element_output = render_output();
            screen.PostEvent(Event::Custom);

            // Render plot
            std::vector<double> x = { 1, 2, 3, 4, 5 };
            std::vector<double> y = { 1, 4, 9, 16, 25 };
            auto plot_element = render_plot(x, y);
            screen.Render(plot_element);
        }
    }
} // namespace calc
