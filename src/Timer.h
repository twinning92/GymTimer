#pragma once
#include "Subject.h"

class Timer : public Subject
{
private:
	Timer();
	static Timer *instance;

public:
	static unsigned int seconds_counter;

	Timer(const Timer &obj) = delete;
	static Timer *getInstance();
	static volatile QueueHandle_t display_queue;
	static volatile QueueHandle_t program_queue;

	static void IRAM_ATTR on_timer();
	static void IRAM_ATTR on_program_timer();
	
	void start_timer(hw_timer_t *hw_timer);
	void stop_timer(hw_timer_t *hw_timer);

	// TODO: This function will just convert hours, minutes, seconds into seconds so that the clock can run.
	static void set_seconds_counter(uint8_t hours, uint8_t minutes, uint8_t seconds);
};