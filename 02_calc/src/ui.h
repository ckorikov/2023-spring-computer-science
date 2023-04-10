#ifndef UI_HPP
#define UI_HPP

#include "ftxui/component/component_base.hpp"     
#include "ftxui/component/component_options.hpp"  
#include "ftxui/component/component.hpp"          
#include "ftxui/dom/elements.hpp"                 
#include "ftxui/component/screen_interactive.hpp" 

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

    // Variable support
    class VariableInput : public InputBase {
    public:
        VariableInput(std::string* value, const std::string& placeholder)
            : InputBase(placeholder), value_(value) {}

        Element Render() override {
            return InputBase::Render() | on_enter([this] {
                auto it = logic_ref.variables.find(*value_);
                if (it != logic_ref.variables.end()) {
                    *value_ = std::to_string(it->second);
                }
            });
        }

    private:
        std::string* value_;
    };

} // namespace calc

#endif // UI_HPP