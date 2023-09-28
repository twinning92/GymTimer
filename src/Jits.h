#pragma once
#include "Program.h"


// The Jits program operates as follows:
// x minutes on, x minutes off
// Rounds are counted instead of decremented.

// There will be a Jits, 5 Jits, 7 Jits where 5/7 are the length of the work rounds.
class Jits : public Program
{
    using Program::Program;

public:
    Jits();
    bool on_notify();
    void set_prog_params() override;
    void init_display_info() override;
};