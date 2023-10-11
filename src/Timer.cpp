#include <Arduino.h>
#include "Timer.h"

Timer *Timer::instance = nullptr;
QueueHandle_t Timer::display_queue = nullptr;
unsigned int Timer::seconds_counter = 0;

Timer::Timer()
{
    display_queue = xQueueCreate(10, sizeof(bool));
    this->instance = nullptr;
};

Timer *Timer::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Timer();
    }
    return instance;
}

void Timer::set_seconds_counter(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    seconds_counter = hours * 3600 + minutes * 60 + seconds;
}

void IRAM_ATTR Timer::on_timer()
{
    bool update_received = true;
    xQueueSendFromISR(display_queue, &update_received, NULL);
    seconds_counter++;

    // TODO: Change it so the final rest period doesn't run.
    // switch (instance->current_phase)
    // {
    // case Phase::WORK:
    //     if (instance->work_seconds > seconds_counter)
    //     {
    //         seconds_counter = 0;
    //         instance->current_phase = Phase::REST;
    //     }
    //     break;
    // case Phase::REST:
    //     if (instance->rest_seconds > seconds_counter)
    //     {
    //         seconds_counter = 0;
    //         instance->current_phase = Phase::WORK;
    //     }
    //     break;
    // }
}

void Timer::start_timer(hw_timer_t *hw_timer)
{
    timerAlarmEnable(hw_timer);
}

void Timer::stop_timer(hw_timer_t *hw_timer)
{
    timerAlarmDisable(hw_timer);
}