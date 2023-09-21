#pragma once
#include "Display.h"

class Clock
{
    public:
    Display* display;

    Clock(Display* clock_display){
        display = clock_display;
    }

    void update_clock_display(int total_seconds_elapsed);
};