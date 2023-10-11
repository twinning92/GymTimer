#include "Countdown_Round.h"

Countdown_Round::Countdown_Round() : Program::Program("dn_rnd")
{
    set_prog_params();
} 

void Countdown_Round::set_prog_params()
{
    this->program_params.need_rounds = true;
    this->program_params.need_work = true;
    this->program_params.need_rest = false;
}

void Countdown_Round::init_display_info()
{
    this->program_display_info.display_rounds = true;
}

void Countdown_Round::on_notify()
{
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

        this->program_display_info.seconds_value = this->work_seconds - this->elapsed_seconds;
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
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 300;
            if (this->num_rounds > 0)
            {
                this->num_rounds--;
            }
            this->program_display_info.rounds_remaining = this->num_rounds;
        }
        break;
    case Phase::FINISHED:
        this->finished_program = true;
        break;
    default:
        break;
    }
}
