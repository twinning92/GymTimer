#pragma once
enum class Phase
{
	HOLD,
	WORK,
	REST,
};
class Timer
{

private:
	Timer();
	static Timer *instance;

public:
	static unsigned int seconds_counter;

	unsigned int work_seconds;
	unsigned int rest_seconds;
	unsigned int num_rounds;

	Timer(const Timer &obj) = delete;
	static Timer *getInstance();
	static QueueHandle_t display_queue;

	static void IRAM_ATTR on_timer();
	void run_session(hw_timer_t *hw_timer);
	void end_session(hw_timer_t *hw_timer);
	void set_work_seconds(unsigned int work_seconds);
	void set_rest_seconds(unsigned int rest_seconds);
	void set_number_rounds(unsigned int num_rounds);
	// TODO: Consider a class or something smarter than doing this.
	static void set_seconds_counter(uint8_t hours, uint8_t minutes, uint8_t seconds);

	unsigned int get_seconds(){
		return seconds_counter;
	}


	Phase current_phase;
};