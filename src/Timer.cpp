#include <Arduino.h>
#include "Timer.h"

Timer *Timer::instance = nullptr;
volatile QueueHandle_t Timer::display_queue = nullptr;
volatile QueueHandle_t Timer::program_queue = nullptr;
unsigned int Timer::seconds_counter = 0;

Timer::Timer()
{
    display_queue = xQueueCreate(10, sizeof(bool));
    if(display_queue == 0)
    {
        Serial.println("Queue create failed");
    }
    program_queue = xQueueCreate(10, sizeof(bool));
    if(program_queue == 0)
    {
        Serial.println("program_queue create failed");
    }
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
    //TODO: don't keep the seconds here probably.
    seconds_counter++;
}
void IRAM_ATTR Timer::on_program_timer()
{
    bool update_received = true;
    xQueueSendFromISR(program_queue, &update_received, NULL);
}
void Timer::start_timer(hw_timer_t *hw_timer)
{
    timerAlarmEnable(hw_timer);
}

void Timer::stop_timer(hw_timer_t *hw_timer)
{
    timerAlarmDisable(hw_timer);
}