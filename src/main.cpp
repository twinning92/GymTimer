#include <Arduino.h>
#include "Timer.h"
#include "Display.h"
#include "Clock.h"

#define IR_RECEIVER_PIN 13

hw_timer_t *hw_timer = nullptr;
Timer *timer;
Display *display;
Clock *clock_69;

void setup()
{
	Serial.begin(115200);
	display = new Display();
	clock_69 = new Clock(display);

	hw_timer = timerBegin(0, 80, true);
	timerAlarmWrite(hw_timer, 1000000, true);
	timerAttachInterrupt(hw_timer, &Timer::on_timer, true);
	timerAlarmEnable(hw_timer);
	timer = Timer::getInstance();

	// Init the clock with the GPS time, but for now... setting the inital time values to 10:45:15.
	timer->set_seconds_counter(23, 59, 50);
}

void loop()
{

	volatile bool update_queued;
	if (xQueueReceive(timer->display_queue, (void *)&update_queued, portMAX_DELAY))
	{
		clock_69->update_clock_display(timer->seconds_counter);
		update_queued = false;
		Serial.print("Seconds elapsed: ");
		Serial.println(timer->seconds_counter);

	}

	// for (int i = 0; i < 14; i++)
	// {
	// 	display->update_display(0, i);
	// 	display->update_display(1, i);
	// 	display->update_display(2, i);
	// 	display->update_display(3, i);
	// 	display->update_display(4, i);
	// 	display->update_display(5, i);
	// 	FastLED.show();
	// }
}
