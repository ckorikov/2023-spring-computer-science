#include "logic.h"
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <cstring>
#include <iostream>

namespace calc
{
    std::vector<std::string> history;

    std::string Logic::process_math()
        {
            if (expression.empty())
            {
                return "";
            }

            std::unordered_map<std::string, double> variables;

            std::istringstream input_ss(expression);
            std::stringstream output_ss;

            std::string a;
            std::string b;
            std::string op;
            input_ss >> a >> op >> b;

            if (op == "=")
            {
                double value = std::stod(b);
                variables[a] = value;
                output_ss << a << op << b;
            }
            else
            {
                if (variables.count(a))
                {
                    a = std::to_string(variables[a]);
                }
                if (variables.count(b))
                {
                    b = std::to_string(variables[b]);
                }

                if (op == "+")
                {
                    double result = std::stod(a) + std::stod(b);
                    output_ss << a << op << b << "=" << result;
                }
                else if (op == "-")
                {
                    double result = std::stod(a) - std::stod(b);
                    output_ss << a << op << b << "=" << result;
                }
                else if (op == "*")
                {
                    double result = std::stod(a) * std::stod(b);
                    output_ss << a << op << b << "=" << result;
                }
                else if (op == "/")
                {
                    double divisor = std::stod(b);
                    if (divisor == 0)
                    {
                        output_ss << "Error: division by zero";
                    }
                    else
                    {
                        double result = std::stod(a) / divisor;
                        output_ss << a << op << b << "=" << result;
                    }
                }
                else if (op == "%")
                {
                    int mod = static_cast<int>(std::stod(b));
                    if (mod == 0)
                    {
                        output_ss << "Error: division by zero";
                    }
                    else
                    {
                        int result = static_cast<int>(std::stod(a)) % mod;
                        output_ss << a << op << b << "=" << result;
                    }
                }
                else if (op == "^")
                {
                    double result = std::pow(std::stod(a), std::stod(b));
                    output_ss << a << op << b << "=" << result;
                }
                else if (op == "!")
                {
                    int num = std::stoi(a);
                    if (num < 0)
                    {
                        output_ss << "Error: factorial of a negative number";
                    }
                    else
                    {
                        int result = 1;
                        for (int i = 1; i <= num; ++i)
                        {
                            result *= i;
                        }
                        output_ss << a << op << "=" << result;
                    }
                }
                else if (op == "p")
                {
                    output_ss << "p=" << 3.14;
                }
                else if (op == "e")
                {
                    output_ss << "e=" << std::exp(1.0);
                }
                else if (op == "c")
                {
                    output_ss << "c=" << 299792458;
                }
                else if (op == "G")
                {
                    output_ss << "G=" << 6.67430e-11;
                }
                else
                {
                    output_ss << "Error: unsupported operation or constant";
                }
            }

            history.push_back(output_ss.str());
            return output_ss.str();
        }

        std::vector<std::string> Logic::get_history() const
        {
            return history;
        }
    
}