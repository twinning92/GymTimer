#pragma once
#include "Display.h"

class Translator
{
    public:
    enum class mode {
        CLOCK,
        TIMER,
    };

    Display* display;
    mode display_mode = mode::CLOCK;

    Translator(Display* operational_display){
        display = operational_display;
    }

    void update_timer_display(int total_seconds_elapsed);

    void update_clock_display(int total_seconds_elapsed);
};