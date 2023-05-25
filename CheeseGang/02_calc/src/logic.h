#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <string> // for char_traits, operator+, string, basic_string
#include <unordered_map> // added for variable support
#include "physics_constants.h"
#include "math_constants.h"

namespace calc
{

    struct Logic
    {
        std::string process_math();
        std::string expression;

        // added for variable support
        std::unordered_map<std::string, int> variables;
        std::vector<std::string> get_history() const;
    };

}

#endif // LOGIC_HPP
