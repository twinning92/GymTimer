#include <Arduino.h>
#include "Timer.h"

Timer *Timer::instance = nullptr;
QueueHandle_t Timer::display_queue = nullptr;
unsigned int Timer::seconds_counter = 0;
unsigned int Timer::test_counter = 0;


Timer::Timer(){
    display_queue = xQueueCreate(10, sizeof(int));
    this->instance = nullptr;
    Serial.println("Constructing Timer!");
};

Timer *Timer::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Timer();
    }
    return instance;
}

void IRAM_ATTR Timer::on_timer()
{
    test_counter = seconds_counter % 10;

    xQueueSendFromISR(display_queue, &test_counter, NULL);
    Serial.println("Interupt!");
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

void Timer::run_session(hw_timer_t *hw_timer)
{
    // if (instance->current_phase == Phase::HOLD)
    // {
    //     this->seconds_counter = 0;
    //     this->current_phase = Phase::WORK;
    //     timerAlarmEnable(hw_timer);
    // }
}

void Timer::end_session(hw_timer_t *hw_timer)
{
    if (instance->current_phase != Phase::HOLD)
    {
        this->current_phase = Phase::HOLD;
        timerAlarmDisable(hw_timer);
    }
}

void Timer::set_work_seconds(unsigned int work_seconds)
{
    this->work_seconds = work_seconds;
}

void Timer::set_rest_seconds(unsigned int rest_seconds)
{
    this->rest_seconds = rest_seconds;
}

void Timer::set_number_rounds(unsigned int num_rounds)
{
    this->num_rounds = num_rounds;
}
