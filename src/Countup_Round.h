#pragma once
#include "Countdown_Round.h"

class Countup_Round : public Countdown_Round
{
    public:
        bool tick() override;
};