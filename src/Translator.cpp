#include "Translator.h"

void Translator::update_timer_display(int total_seconds_elapsed)
{

    display->update_display(0, (unsigned char)total_seconds_elapsed % 10);

    if ((int)display->digits[1].current_value != (total_seconds_elapsed / 10) % 6)
    {
        display->update_display(1, (unsigned char)total_seconds_elapsed / 10 % 6);
    }

    if ((int)display->digits[2].current_value != (total_seconds_elapsed / 60) % 10)
    {
        display->update_display(2, (unsigned char)(total_seconds_elapsed / 60) % 10);
    }

    if ((int)display->digits[3].current_value != (total_seconds_elapsed / 600) % 6)
    {
        display->update_display(3, (unsigned char)(total_seconds_elapsed / 600) % 6);
    }

    if ((int)display->digits[4].current_value != (total_seconds_elapsed / 3600) % 10)
    {
        display->update_display(4, (unsigned char)(total_seconds_elapsed / 3600) % 10);
    }
    if ((int)display->digits[5].current_value != (total_seconds_elapsed / 36000) % 10)
    {
        display->update_display(5, (unsigned char)(total_seconds_elapsed / 36000) % 10);
    }
}

/*

    position_0 = total_seconds_elapsed % 10;

    position_1 = (total_seconds_elapsed / 10) % 6;

    position_2 = (total_seconds_elapsed / 60) % 10;

    position_3 = (total_seconds_elapsed / 600) % 6;

    position_4 = (total_seconds_elapsed / 3600 ) % 10;

    position_5 = (total_seconds_elapsed / 36000) % 10;
*/