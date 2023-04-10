#ifndef UI_HPP
#define UI_HPP

#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/component.hpp"          // for Input, Renderer, Vertical
#include "ftxui/dom/elements.hpp"                 // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive

#include "logic.h"

namespace calc
{

    using namespace ftxui;

    class UI
    {
    public:
        UI(Logic &logic) : logic_ref{logic}
        {
            expression_input_box = Input(&logic.expression, "...");
            element_output = hbox(text(""));
        }

        void start();

    private:
        Element render_input();
        Element render_output();
        bool process_events(Event event);

    private:
        Logic &logic_ref;

        Component expression_input_box;
        Element element_output;
        ScreenInteractive screen = ScreenInteractive::Fullscreen();
    };

}

#endif // UI_HPP