#pragma once

#include "Display.h"
class Program
    {
    public:
        Display &display;
        const char *program_name;

        Program(Display &display);
        void setup_program();
    };