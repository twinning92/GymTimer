#pragma once
#include "Display.h"
#include "Program.h"

#define NUM_PROGRAMS 4

class Menu
{
public:
    Program programs[NUM_PROGRAMS];
    Program selected_program;

    Menu();
    void select_program();
    void cycle_menu();
};