#pragma once
#include "Program.h"

class Countdown_Round : public Program
{
public:
    Countdown_Round();
    void set_prog_params() override;
    void init_display_info() override;

    void on_notify() override;
};