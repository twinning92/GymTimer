#include <Arduino.h>

#include <DIYables_IRcontroller.h>
#include <FastLED.h>

// put function declarations here:

#define IR_RECEIVER_PIN 13

#define NUM_MENU_ITEMS 3

enum ProgramState
{
	FIRST_BOOT,
	IDLE,
	DISPLAY_MENU,
	CONFIGURE_COUNTDOWN,
	RUNNING,
};

enum MenuItem
{
	COUNTDOWN,
	COUNTDOWN_WITH_ROUNDS,
	SET_TIME,
};

enum Phase
{
	HOLD,
	WORK,
	REST,
};

DIYables_IRcontroller_21 irController(IR_RECEIVER_PIN, 200); // debounce time is 200ms
hw_timer_t *hw_timer = NULL;

void setup()
{
	irController.begin();

	hw_timer = timerBegin(0, 80, true);
	timerAlarmWrite(hw_timer, 1000000, true);
	// Configure interrupt for IR input
}

void loop()
{
}

class Timer
{

private:
	static Timer *instance;
	static unsigned int seconds_counter;

	Timer(unsigned int work_seconds, unsigned int rest_seconds, unsigned int num_rounds)
	{
		this->work_seconds = work_seconds;
		this->rest_seconds = rest_seconds;
		this->num_rounds = num_rounds;
		current_phase = HOLD;

		instance = this;
		seconds_counter = 0;
		timerAttachInterrupt(hw_timer, &on_timer, true);
	}

public:
	Timer(const Timer &obj) = delete;

	unsigned int work_seconds;
	unsigned int rest_seconds;
	unsigned int num_rounds;
	Phase current_phase;

	static Timer *get_instance()
	{
		if (instance == nullptr)
		{
			instance = new Timer(0, 0, 0);
			return instance;
		}
		else
		{
			return instance;
		}
	}

	static void IRAM_ATTR on_timer()
	{
		seconds_counter++;
		switch (instance->current_phase)
		{
		case WORK:
			if (instance->work_seconds >= seconds_counter)
			{
				seconds_counter = 0;
				instance->current_phase = REST;
			}
			break;
		case REST:
			if (instance->rest_seconds >= seconds_counter)
			{
				seconds_counter = 0;
				instance->current_phase = WORK;
			}
			break;
		}
	}

	void run_session()
	{
		seconds_counter = 0;
		current_phase = WORK;
		timerAlarmEnable(hw_timer);
	}
};

class Display
{
public:
	unsigned char second;
	unsigned char ten_second;
	unsigned char minute;
	unsigned char ten_minute;
	unsigned char alpha_1;
	unsigned char alpha_2;

	std::array<int, 14> segment_chars = {
		0b1111111111111111111111110000,
		0b1111000000000000000000001111,
		0b0000111111110000111111111111,
		0b1111111100000000111111111111,
		0b1111000000001111000011111111,
		0b1111111100001111111100001111,
		0b1111111111111111111100001111,
		0b1111000000000000111111110000,
		0b1111111111111111111111111111,
		0b1111111100001111111111111111,
		0b0000111111111111000000001111,
		0b1111000011111111111111111111,
		0b1111111111111111000000001111,
		0b0000111111111111111100001111,
	};

	

	void clear_display(){

	}
	/*

	1	 2	  3	   4    5    6    7
	1111 1111 1111 1111 1111 1111 0000	// 0

	1111 0000 0000 0000 0000 0000 1111  // 1

	0000 1111 1111 0000 1111 1111 1111  // 2

	1111 1111 0000 0000 1111 1111 1111  // 3

	1111 0000 0000 1111 0000 1111 1111  // 4

	1111 1111 0000 1111 1111 0000 1111  // 5

	1111 1111 1111 1111 1111 0000 1111  // 6

	1111 0000 0000 0000 1111 1111 0000  // 7

	1111 1111 1111 1111 1111 1111 1111  // 8

	1111 1111 0000 1111 1111 1111 1111  // 9

	0000 1111 1111 1111 0000 0000 1111  // t

	1111 0000 1111 1111 1111 1111 1111  // A

	1111 1111 1111 1111 0000 0000 1111  // b

	0000 1111 1111 1111 1111 0000 1111  // E
	*/
};