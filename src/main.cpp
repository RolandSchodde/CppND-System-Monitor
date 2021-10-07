#include "iostream"
#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  // start debug
  // std::cout << "main" << std::endl;
  // end debug
  NCursesDisplay::Display(system);
}