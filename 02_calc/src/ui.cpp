#include "ui.h"
#include "gnuplot-iostream.h"

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

    std::vector<double> x_vals;
    std::vector<double> y_vals;
    {
        for (double x = 0; x < 10; x += 0.1)
        {
            x_vals.push_back(x);
            y_vals.push_back(sin(x));
        }

        Gnuplot gp;
        gp << "plot '-' with lines\n";
        gp.send1d(boost::make_tuple(x_vals, y_vals));
    }

    Element UI::render_input()
    {
        return hbox(text(" Expression: "), expression_input_box->Render());
    }

    Element UI::render_output()
    {
        if (logic_ref.expression == "plot")
        {
            Gnuplot gp;
            gp << "set term dumb\n";
            gp << "plot sin(x)\n";
            std::string output;
            gp >> output;
            return hbox(text(output));
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
        else if (event == Event::Custom)
        {
            if (logic_ref.expression == "plot")
            {
                element_output = render_output();
                logic_ref.expression.clear();
                return true;
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

        screen.PostEvent(Event::Custom);
        screen.Loop(renderer);
    }

} // namespace calc
