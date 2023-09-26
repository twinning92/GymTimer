#pragma once
#include "Program.h"

class Countdown_Round : public Program
{
    public:
    using Program::Program;
    void set_prog_params() override; 
    void set_prog_run() override;
    
    void start() override;
    virtual bool tick() override;
};