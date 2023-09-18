#include <Arduino.h>
#include "Timer.h"
#include "Display.h"

#define IR_RECEIVER_PIN 13

hw_timer_t *hw_timer = nullptr;
Timer *timer;
Display *display;

void setup()
{
	Serial.begin(115200);
	display = new Display();

	hw_timer = timerBegin(0, 80, true);
	timerAlarmWrite(hw_timer, 1000000, true);
	timerAttachInterrupt(hw_timer, &Timer::on_timer, true);
	// timerAlarmEnable(hw_timer);
	timer = Timer::getInstance();
}

void loop()
{
	// volatile int update;
	// if (xQueueReceive(timer->display_queue, (void *) &update, portMAX_DELAY))
	// {
	// 	display->update_display(0, update);
	// }
	for (int i = 0; i < 14; i++)
	{
		for (int j = 0; j < 4; j++)
		{

			display->update_display(j, i);
		}
		delay(500);
	}
}
