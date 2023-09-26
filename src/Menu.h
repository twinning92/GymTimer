#pragma once
#include <Arduino.h>
#include "Program.h"
#include "Countdown_Round.h"

class Menu
{
public:
    Menu() : programs{new Countdown_Round("dn_rnd"), nullptr, nullptr, nullptr}{};
    const std::string get_program_string(uint8_t program_index);
    Program* select_program(uint8_t program_index);

    std::array<Program*, 4> programs;
private:
};