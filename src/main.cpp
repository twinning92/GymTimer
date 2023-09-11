#include <Arduino.h>
#include "Timer.h"
#include "Display.h"
#include "Translator.h"

#define IR_RECEIVER_PIN 13

#define NUM_MENU_ITEMS 3

hw_timer_t *hw_timer = nullptr;
Timer *timer;
Display *display;
Translator *translator;

void setup()
{
	// timer = Timer::getInstance();
	display = new Display();
	// translator = new Translator(display);

	// hw_timer = timerBegin(0, 80, true);
	// timerAlarmWrite(hw_timer, 1000000, true);
	// timerAttachInterrupt(hw_timer, &Timer::on_timer, true);
}

void loop()
{
	// int update;
	// if (xQueueReceive(timer->display_queue, &update, portMAX_DELAY))
	//{
	//	translator->update_timer_display(timer->seconds_counter);
	// }

	display->update_display(0, '0');
	delay(250);
	display->update_display(0, '1');
	delay(250);
	display->update_display(0, '2');
	delay(250);
	display->update_display(0, '3');
	delay(250);
	display->update_display(0, '4');
	delay(250);
	display->update_display(0, '5');
	delay(250);
	display->update_display(0, '6');
	delay(250);
	display->update_display(0, '7');
	delay(250);
	display->update_display(0, '8');
	delay(250);
	display->update_display(0, '9');
	delay(250);

}
