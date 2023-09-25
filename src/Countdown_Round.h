#pragma once
#include "Program.h"

class Countdown_Round : public Program
{
    public:
    using Program::Program;
    void set_prog_params() override; 
    void set_prog_run() override;
    struct Program::prog_params get_prog_params() override;
    struct Program::prog_run get_running_info() override;
    
    void start() override;
    bool tick() override;
};