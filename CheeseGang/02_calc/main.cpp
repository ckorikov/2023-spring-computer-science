#include <ui.h>
#include <logic.h>

int main()
{
  calc::Logic calculator;
  calc::UI ui(calculator);
  std::vector<std::string> history = calculator.get_history();
  std::cout << "История операций:\n";
  for (const auto& entry : history)
  {
      std::cout << entry << '\n';
  }
  

  ui.start();

  return 0;
}
