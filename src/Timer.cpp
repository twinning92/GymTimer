#include <Arduino.h>
#include "Timer.h"

Timer *Timer::instance = nullptr;
volatile QueueHandle_t Timer::display_queue = nullptr;
unsigned int Timer::seconds_counter = 0;

Timer::Timer()
{
    display_queue = xQueueCreate(10, sizeof(bool));
    if(display_queue == 0)
    {
        Serial.println("Queue create failed");
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
    seconds_counter++;
}

void Timer::start_timer(hw_timer_t *hw_timer)
{
    timerAlarmEnable(hw_timer);
}

void Timer::stop_timer(hw_timer_t *hw_timer)
{
    timerAlarmDisable(hw_timer);
}