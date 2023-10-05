#pragma once
#include <Arduino.h>
#include "Program.h"
#include "Countdown_Round.h"
#include "Jits.h"
#include "Countup_Round.h"

#define NUM_PROGRAMS 4


class Menu
{
public:
    Menu() : programs{new Countdown_Round("dn rnd"), new Jits("Jits"), new Countdown_Round("up rnd"), new Jits("jitsu")}{};
    String get_program_string(uint8_t program_index);
    Program* select_program(uint8_t program_index);

    std::array<Program*, NUM_PROGRAMS> programs;
private:
};