#pragma once
#include "Countdown_Round.h"

class Menu
{
public:
    Menu();
    const char* get_program_string(uint8_t program_index);
    Program* select_program(uint8_t program_index);

    std::array<Program *, 4> programs;
private:
};