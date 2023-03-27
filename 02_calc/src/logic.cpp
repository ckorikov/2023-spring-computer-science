#include "logic.h"
#include <cmath>
#include <sstream>
#include <vector>

namespace calc
{
    std::vector<std::string> history;   // история операций

    std::string Logic::process_math()
    {
        if (expression.empty())
        {
            return "";
        }

        std::istringstream input_ss(expression);
        std::stringstream output_ss;

        int a;
        char op;
        input_ss >> a >> op;

        switch (op)
        {
            case '+':
                {
                    int b;
                    input_ss >> b;
                    output_ss << a << op << b << "=" << a + b;
                    break;
                }
            case '-':
                {
                    int b;
                    input_ss >> b;
                    output_ss << a << op << b << "=" << a - b;
                    break;
                }
            case '*':
                {
                    int b;
                    input_ss >> b;
                    output_ss << a << op << b << "=" << a * b;
                    break;
                }    
            case '/':
                {
                    int b;
                    input_ss >> b;
                    output_ss << a << op << b << "=" << a / b;
                    break;
                }
            case '^':
                {
                    if (input_ss.peek() == '2')
                    {
                        input_ss.ignore(); // пропускаем символ '^'

                        output_ss << a << "^2" << "=" << pow(a, 2);
                    }
                    else
                    {
                        output_ss << "Неизвестная операция";
                    }
                    break;
                }
            case 's':
                {
                    if (input_ss.peek() == 'q')
                    {
                        input_ss.ignore(4); // пропускаем символы 's', 'q', 'r', 't'

                        output_ss << "sqrt(" << a << ")" << "=" << sqrt(a);
                    }
                    else
                    {
                        output_ss << "Неизвестная операция";
                    }
                    break;
                }
            default:
                {
                    output_ss << "Неизвестная операция";
                    break;
                }
        }

        history.push_back(output_ss.str());    // добавляем операцию в историю
        return output_ss.str();
    }

    void Logic::print_history()
    {
        std::cout << "История операций:\n";
        for (auto op : history)
        {
            std::cout << op << '\n';
        }
    }
}
}
