#pragma once

class Timer
{
enum Phase
{
	HOLD,
	WORK,
	REST,
};
private:
	Timer();
	static Timer *instance;
	static unsigned int seconds_counter;

	unsigned int work_seconds;
	unsigned int rest_seconds;
	unsigned int num_rounds;

public:
	Timer(const Timer &obj) = delete;
	static Timer *getInstance();

	static void IRAM_ATTR on_timer();
	void run_session(hw_timer_t *hw_timer);
	void end_session(hw_timer_t *hw_timer);
	void set_work_seconds(unsigned int work_seconds);
	void set_rest_seconds(unsigned int rest_seconds);
	void set_number_rounds(unsigned int num_rounds);


	Phase current_phase;
};