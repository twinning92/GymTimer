#include "Menu.h"

const std::string Menu::get_program_string(uint8_t program_index)
{
    return programs[program_index]->get_name();
}

Program * Menu::select_program(uint8_t program_index)
{
    return programs[program_index];
}