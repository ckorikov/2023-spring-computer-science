#include "logic.h"
#include <sstream>
#include <unordered_map> // added for variable support

namespace calc
{
    std::string Logic::process_math()
    {
        if (expression.empty())
        {
            return "";
        }

        // added code for variable support
        std::unordered_map<std::string, int> variables;
        variables["pi"] = 3.14159265358979323846;
        variables["e"] = 2.71828182845904523536;

        std::istringstream input_ss(expression);
        std::stringstream output_ss;

        std::string token;
        int result = 0;
        bool is_first_number = true;

        while (std::getline(input_ss, token, ' '))
        {
            if (variables.find(token) != variables.end())
            {
                token = std::to_string(variables[token]);
            }

            std::stringstream number_stream(token);
            int number = 0;

            if (number_stream >> number)
            {
                if (is_first_number)
                {
                    result = number;
                    is_first_number = false;
                }
                else
                {
                    switch (last_operator)
                    {
                    case '+':
                        result += number;
                        break;
                    case '-':
                        result -= number;
                        break;
                    case '*':
                        result *= number;
                        break;
                    case '/':
                        result /= number;
                        break;
                    default:
                        break;
                    }
                }
            }
            else if (token.length() == 1 && token.find_first_of("+-*/") != std::string::npos)
            {
                last_operator = token[0];
            }
        }

        output_ss << result;
        return output_ss.str();
    }
}