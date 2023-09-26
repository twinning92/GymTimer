#include "Jits.h"


// Pre programmed setup. No configuration.
void Jits::set_prog_params()
{
    this->program_params.need_rounds = false;
    this->program_params.need_work = true;
    this->program_params.need_rest = true;
}

void Jits::set_display_info()
{
    this->program_display_info.display_rounds = true;
}

void Jits::set_work_seconds(int work_seconds_)
{
    this->work_seconds = work_seconds_;
}

void Jits::set_rest_seconds(int rest_seconds_)
{
    this->rest_seconds = rest_seconds_;
}

bool Jits::tick()
{
    return false;
}