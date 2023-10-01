#include "Menu.h"

const String Menu::get_program_string(uint8_t program_index)
{
    return programs[program_index]->get_name();
}

Program * Menu::select_program(uint8_t program_index)
{
    return programs[program_index];
}