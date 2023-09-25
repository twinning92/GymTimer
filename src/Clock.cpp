#include "Clock.h"

void Clock::update_clock_display(int total_seconds_elapsed)
{
    uint8_t low_seconds = total_seconds_elapsed % 10;
    uint8_t high_seconds = total_seconds_elapsed / 10 % 6;

    uint8_t total_minutes = (total_seconds_elapsed / 60) % 60;
    uint8_t low_minutes = total_minutes % 10;
    uint8_t high_minutes = total_minutes / 10;

    uint8_t total_hours = (total_seconds_elapsed / 3600) % 24;
    uint8_t low_hours = total_hours % 10;
    uint8_t high_hours = total_hours / 10;

    display->update_display(4, low_seconds);
    display->update_display(5, high_seconds);
    display->update_display(0, low_minutes);
    display->update_display(1, high_minutes);
    display->update_display(2, low_hours);
    display->update_display(3, high_hours);
    display->push_to_display();
}

/*
    position_0 = total_seconds_elapsed % 10;
    position_1 = (total_seconds_elapsed / 10) % 6;
    position_2 = (total_seconds_elapsed / 60) % 10;
    position_3 = (total_seconds_elapsed / 600) % 6;
    position_4 = (total_seconds_elapsed / 3600 ) % 10;
    position_5 = (total_seconds_elapsed / 36000) % 10;
*/