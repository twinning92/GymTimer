#include "Translator.h"

void Translator::update_clock_display(int total_seconds_elapsed)
{

    display->update_display(0, total_seconds_elapsed % 10);

    display->update_display(1, total_seconds_elapsed / 10 % 6);

    display->update_display(2, (total_seconds_elapsed / 60) % 10);

    display->update_display(3, (total_seconds_elapsed / 600) % 6);

    // TODO: This does not handle 24 hour time correctly.

    display->update_display(4, (total_seconds_elapsed / 3600) % 10);

    display->update_display(5, (total_seconds_elapsed / 36000) % 3);

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