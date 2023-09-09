#include "Timer.h"

Timer *Timer::instance = nullptr;

Timer *Timer::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Timer();
    }
    return instance;
}

static void IRAM_ATTR on_timer()
{
    // TODO: Change it so the final rest period doesn't run.
    seconds_counter++;
    switch (instance->current_phase)
    {
    case WORK:
        if (instance->work_seconds >= seconds_counter)
        {
            seconds_counter = 0;
            instance->current_phase = REST;
        }
        break;
    case REST:
        if (instance->rest_seconds >= seconds_counter)
        {
            seconds_counter = 0;
            instance->current_phase = WORK;
        }
        break;
    }
}

void run_session(hw_timer_t* hw_timer)
{
    this.seconds_counter = 0;
    this.current_phase = WORK;
    timerAlarmEnable(hw_timer);
}

void end_session(hw_timer_t* hw_timer)
{
    this.current_phase = HOLD;
    timerAlarmDisable(hw_timer);
}

void set_work_seconds(unsigned int work_seconds)
{
    this->work_seconds = work_seconds;
}

void set_rest_seconds(unsigned int rest_seconds)
{
    this->rest_seconds = rest_seconds;
}

void set_number_rounds(unsigned int num_rounds)
{
    this->num_rounds = num_rounds;
}
