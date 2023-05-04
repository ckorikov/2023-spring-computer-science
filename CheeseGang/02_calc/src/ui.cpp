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
