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
    bool tick();
    void set_prog_params() override;
    void set_display_info() override;
    void set_work_seconds(int work_seconds_) override;
    void set_rest_seconds(int rest_seconds_) override;
    void start() override;
};