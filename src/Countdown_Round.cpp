#include "Countdown_Round.h"

void Countdown_Round::set_prog_params()
{
    this->program_params.need_rounds = true;
    this->program_params.need_work = true;
    this->program_params.need_rest = false;
}

void Countdown_Round::set_display_info()
{
    this->program_display_info.display_rounds = true;
}

/// @brief Sets phase to TEN_SECOND_START.
void Countdown_Round::start()
{
    this->program_phase = Phase::TEN_SECOND_TO_START;
}

bool Countdown_Round::tick()
{
    bool finished = false;
    switch (this->program_phase)
    {
    case Phase::TEN_SECOND_TO_START:
        this->program_display_info.seconds_value = this->start_seconds;
        this->start_seconds--;
        if (this->start_seconds <= 3)
        {
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 100;
        }
        if (this->start_seconds == 0)
        {
            this->program_phase = Phase::WORK;
            this->program_display_info.beep_milliseconds = 300;
            this->program_display_info.beep = false;
        }
        break;
    case Phase::WORK:
        this->program_display_info.currently_working = true;
        this->program_display_info.seconds_value = this->work_seconds - this->elapsed_time;
        this->program_display_info.rounds_remaining = this->num_rounds;

        this->elapsed_time++;
        if (work_seconds - elapsed_time <= 3)
        {
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 100;
        }
        if (work_seconds == elapsed_time)
        {
            elapsed_time = 0;
            this->program_display_info.beep_milliseconds = 300;
            this->program_display_info.beep = false;
            this->num_rounds--;
            this->program_display_info.rounds_remaining = this->num_rounds;
        }
        break;
    case Phase::FINISHED:
        finished = true;
        break;
    default:
        break;
    }
    return finished;
}
