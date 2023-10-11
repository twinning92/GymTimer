#include "Jits.h"

Jits::Jits() : Program::Program("Jits")
{
    set_prog_params();
}

// Pre programmed setup. No configuration. For Jits we explicitly set num rounds to 0 so we can increment and count
void Jits::set_prog_params()
{
    this->program_params.need_rounds = false;
    this->set_num_rounds(0);
    this->program_params.need_work = true;
    this->program_params.need_rest = true;
}

void Jits::init_display_info()
{
    this->program_display_info.display_rounds = true;
}


void Jits::on_notify()
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
        this->elapsed_seconds++;
        if ((this->seconds_to_work - this->elapsed_seconds) <= 3)
        {
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 100;
        }
        if (this->seconds_to_work >= this->elapsed_seconds)
        {
            this->elapsed_seconds = 0;
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 300;
            this->program_phase = Phase::REST;
        }
        break;
    case Phase::REST:
        this->program_display_info.beep = false;
        this->program_display_info.seconds_display_val = elapsed_seconds;
        this->elapsed_seconds++;
        if ((this->seconds_to_rest - this->elapsed_seconds) <= 3)
        {
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 100;
        }
        if (this->seconds_to_rest >= this->elapsed_seconds)
        {
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 300;
            this->total_num_rounds++;
            this->program_display_info.rounds_remaining = this->total_num_rounds;
            this->elapsed_seconds = 0;
            this->program_phase = Phase::WORK;
        }
        break;

        if (this->total_num_rounds <= 0)
        {
            this->finished_program = true;
        }
    }
}