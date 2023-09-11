#pragma once
#include "Display.h"
#include "Program.h"

#define NUM_PROGRAMS 4

class Menu
{
public:
    Display &display;
    Program programs[NUM_PROGRAMS];
    Program selected_program;

    Menu(Display &display);
    void select_program();
    void cycle_menu();
};