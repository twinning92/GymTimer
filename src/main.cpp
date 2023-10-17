#include <Arduino.h>
#include <IRremote.hpp>

#include "IR_Signal.h"
#include "RTC.h"
#include "Menu.h"
#include "Buzzer.h"
#include "Display.h"

// IR Receiver Handling
IR_Signal ir;
// Real Time Clock handling
RTC Rtc;
// Menu handling
Menu menu;
// Buzzer handling
Buzzer buzzer;
// Display Inferface Handling
Display display;

// 100msec timer callback
static void on_timer(void) {
	menu.on_timer();
	buzzer.on_timer();
}

void start_main_timer() {
	uint16_t div = getApbFrequency() / 10000; // clock frequency / base frequency eg: 80,000,000 / 10,000
	hw_timer_t *hw_timer = timerBegin(0, div, true);
	timerAlarmWrite(hw_timer, 10000, true); // fire an event at 10hz
	timerAttachInterrupt(hw_timer, on_timer, true);
	timerAlarmEnable(hw_timer);
}

void setup()
{
	// start uart
	Serial.begin(115200);
	// start 100msec(10hz) timer with a 10khz base 
	start_main_timer();
	// start the IR receiver
	ir.begin();
	// prepare everything
	menu.reset();
}

void loop()
{
	menu.tick();
}



