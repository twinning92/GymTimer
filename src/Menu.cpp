#include "Menu.h"

Menu::Menu()
{
    programs[0] = new Countdown_Round("cntdn");
    programs[1] = new Countdown_Round("cntrnd");
    programs[2] = new Countdown_Round("tabata");
    programs[3] = new Countdown_Round("clock");
}

const char* Menu::get_program_string(uint8_t program_index)
{
    return programs[program_index]->get_name();
}

Program * Menu::select_program(uint8_t program_index)
{
    return programs[program_index];
}