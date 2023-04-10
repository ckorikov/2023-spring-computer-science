#include <ui.h>
#include <logic.h>

int main()
{
  calc::Logic logic;
  calc::UI ui(logic);

  ui.start();

  return 0;
}
