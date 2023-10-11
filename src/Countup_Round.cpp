#include "Countup_Round.h"

void Countup_Round::on_notify()
{
    switch (this->program_phase)
    {
    case Phase::TEN_SECOND_TO_START:
        this->program_display_info.display_rounds = false;

        this->program_display_info.seconds_display_val = this->elapsed_seconds;
        this->elapsed_seconds++;
        if (this->ten_second_countdown - this->elapsed_seconds <= 3)
        {
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 100;
        }
        if (this->ten_second_countdown <= this->elapsed_seconds)
        {
            this->program_phase = Phase::WORK;
            this->program_display_info.display_rounds = true;
            this->program_display_info.beep_milliseconds = 300;
            this->program_display_info.beep = false;
            this->elapsed_seconds = 0;
        }
        break;
    case Phase::WORK:
        this->program_display_info.beep = false;

        this->program_display_info.seconds_display_val = elapsed_seconds;
        this->program_display_info.rounds_remaining = this->total_num_rounds;

        this->elapsed_seconds++;
        if (seconds_to_work - elapsed_seconds <= 3)
        {
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 100;
        }
        if (seconds_to_work >= elapsed_seconds)
        {
            elapsed_seconds = 0;
            this->program_display_info.beep_milliseconds = 300;
            this->program_display_info.beep = true;
            if (this->total_num_rounds > 0)
            {
                this->total_num_rounds--;
            }
            this->program_display_info.rounds_remaining = this->total_num_rounds;
        }
        if (this->total_num_rounds <= 0)
        {
            this->finished_program = true;
        }
        break;
    }
}