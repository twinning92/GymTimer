#pragma once
#include "Program.h"

class Countup_Round : public Program
{
    using Program::Program;

    public:
        void on_notify() override;
};