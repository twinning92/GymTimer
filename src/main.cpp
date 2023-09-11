#include <Arduino.h>
#include "Timer.h"
#include "Display.h"
#include "Translator.h"

#define IR_RECEIVER_PIN 13

hw_timer_t *hw_timer = nullptr;
Timer *timer;
Display *display;

void setup()
{
	timer = Timer::getInstance();
	display = new Display();

	hw_timer = timerBegin(0, 80, true);
	timerAlarmWrite(hw_timer, 1000000, true);
	timerAttachInterrupt(hw_timer, &Timer::on_timer, true);
}

void loop()
{
	// int update;
	// if (xQueueReceive(timer->display_queue, &update, portMAX_DELAY))
	//{
	//	translator->update_timer_display(timer->seconds_counter);
	// }

}
