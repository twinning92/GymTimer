#pragma once
#include "Program.h"

class Countdown_Round : public Program
{
public:
    using Program::Program;
    void set_prog_params() override;
    void init_display_info() override;

    void on_notify() override;
};