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
    // Redundant, as I set to false first during 10 seconds start phase, then turn it back on.
    this->program_runner.show_rounds = true;
}

void Countdown_Round::on_notify()
{
    switch (this->program_runner.program_phase)
    {
    case Phase::TEN_SECOND_TO_START:
        this->program_runner.seconds_value--;
        if (this->program_runner.seconds_value > 3)
        {
            Serial.printf("Beep!\n");
        }
        if (this->program_runner.seconds_value <= 0)
        {
            Serial.printf("BEEEEEP!\n");
            this->program_runner.seconds_value = this->program_runner.total_work_time;
            this->program_runner.program_phase = Phase::WORK;
            this->program_runner.show_rounds = true;
            this->program_runner.currently_working = true;
        }
        break;
    case Phase::WORK:
        this->program_runner.seconds_value--;
        if (this->program_runner.rounds_value >= 0)
        {
            if (this->program_runner.seconds_value > 3)
            {
                Serial.printf("Beep!\n");
            }
            if (this->program_runner.seconds_value <= 0)
            {
                this->program_runner.seconds_value = this->seconds_to_work;
                this->program_runner.rounds_value--;
                Serial.printf("Rounds Value= %d\n", this->program_runner.rounds_value);
            }
        }
        else
        {
            Serial.printf("BEEEEEEEEEEEEEEEEEEEEEP!\n");
            this->program_runner.finished_program = true;
            this->program_runner.currently_working = false;
            this->program_runner.program_phase = Phase::FINISHED;
        }

        break;
    }

    //     break;
    // case Phase::WORK:

    //     break;
    // case Phase::FINISHED:

    //     break;
    // default:
    //     break;
    // }
}
