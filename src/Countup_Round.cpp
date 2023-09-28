#include "Countup_Round.h"

bool Countup_Round::on_notify()
{
    bool finished = false;
    switch (this->program_phase)
    {
    case Phase::TEN_SECOND_TO_START:
        this->program_display_info.display_rounds = false;

        this->program_display_info.seconds_value = this->elapsed_seconds;
        this->elapsed_seconds++;
        if (this->start_seconds - this->elapsed_seconds <= 3)
        {
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 100;
        }
        if (this->start_seconds <= this->elapsed_seconds)
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

        this->program_display_info.seconds_value = elapsed_seconds;
        this->program_display_info.rounds_remaining = this->num_rounds;

        this->elapsed_seconds++;
        if (work_seconds - elapsed_seconds <= 3)
        {
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 100;
        }
        if (work_seconds >= elapsed_seconds)
        {
            elapsed_seconds = 0;
            this->program_display_info.beep_milliseconds = 300;
            this->program_display_info.beep = true;
            if (this->num_rounds > 0)
            {
                this->num_rounds--;
            }
            this->program_display_info.rounds_remaining = this->num_rounds;
        }
        if (this->num_rounds <= 0)
        {
            finished = true;
        }
        break;
    }
    return finished;
}