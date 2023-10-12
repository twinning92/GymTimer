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
    this->program_runner.show_rounds = true;
}


void Jits::on_notify()
{}