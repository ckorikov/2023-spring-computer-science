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