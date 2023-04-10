#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <string> // for char_traits, operator+, string, basic_string
#include <unordered_map> // added for variable support

namespace calc
{

    struct Logic
    {
        std::string process_math();
        std::string expression;

        // added for variable support
        std::unordered_map<std::string, int> variables;
    };

}

#endif // LOGIC_HPP
#endif // LOGIC_HPP