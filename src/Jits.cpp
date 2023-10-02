#include "Jits.h"

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
        this->elapsed_seconds++;
        if ((this->work_seconds - this->elapsed_seconds) <= 3)
        {
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 100;
        }
        if (this->work_seconds >= this->elapsed_seconds)
        {
            this->elapsed_seconds = 0;
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 300;
            this->program_phase = Phase::REST;
        }
        break;
    case Phase::REST:
        this->program_display_info.beep = false;
        this->program_display_info.seconds_value = elapsed_seconds;
        this->elapsed_seconds++;
        if ((this->rest_seconds - this->elapsed_seconds) <= 3)
        {
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 100;
        }
        if (this->rest_seconds >= this->elapsed_seconds)
        {
            this->program_display_info.beep = true;
            this->program_display_info.beep_milliseconds = 300;
            this->num_rounds++;
            this->program_display_info.rounds_remaining = this->num_rounds;
            this->elapsed_seconds = 0;
            this->program_phase = Phase::WORK;
        }
        break;

        if(this->num_rounds <= 0)
        {
            this->finished_program = true;
        }
    }
}