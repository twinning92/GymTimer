#include "Countdown_Round.h"

void Countdown_Round::set_prog_params()
{
    this->program_params.need_rounds = true;
    this->program_params.need_work = true;
    this->program_params.need_rest = false;
}

void Countdown_Round::set_prog_run()
{
    this->program_run.display_rounds = true;
}

struct Program::prog_params Countdown_Round::get_prog_params()
{
    return this->program_params;
}

struct Program::prog_run Countdown_Round::get_running_info()
{
    return this->program_run;
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
        this->program_run.seconds_remaining = start_seconds;
        this->start_seconds--;
        if (start_seconds <= 3)
        {
            this->program_run.beep = true;
            this->program_run.beep_milliseconds = 100;
        }
        if (start_seconds == 0)
        {
            this->program_phase = Phase::WORK;
            this->program_run.beep_milliseconds = 300;
            this->program_run.beep = false;
        }
        break;
    case Phase::WORK:
        this->program_run.currently_working = true;
        this->program_run.seconds_remaining = work_seconds;
        this->program_run.rounds_remaining = this->num_rounds;

        this->work_seconds--;
        if (work_seconds <= 3)
        {
            this->program_run.beep = true;
            this->program_run.beep_milliseconds = 100;
        }
        if (work_seconds == 0)
        {
            this->program_run.beep_milliseconds = 300;
            this->program_run.beep = false;
            this->num_rounds--;
            this->program_run.rounds_remaining = this->num_rounds;
        }
        if (this->num_rounds == 0)
        {
            finished = true;
        }
        break;
    }
    return finished;
}
