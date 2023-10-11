#include <Arduino.h>
#include <IRremote.hpp>

#include "Timer.h"
#include "Display.h"
#include "Menu.h"
#include "Clock.h"
#include "Countdown_Round.h"
#include "IR_Signal.h"

#include "RTC.h"

enum class Function_State
{
	IDLE,
	NAVIGATING_MENU,
	CONFIGURING_PROGRAM,
	READY_TO_START,
	RUNNING_TIMER,
};

hw_timer_t *hw_timer = nullptr;
TickType_t x_ticks_to_wait = pdMS_TO_TICKS(150);
Timer *timer = nullptr;
Display *display = nullptr;
Menu *menu = nullptr;
Clock *clock_69 = nullptr;
RTC *rtc = nullptr;
IR_Signal *ir = nullptr;
uint8_t program_index = 0;
#define program_max (menu->programs.size())

Function_State state;

uint16_t control_work_input(uint16_t *ir_command, CRGB colour, CRGB selected_colour = CRGB::Orange);
uint8_t control_round_input(uint16_t *ir_command);
void print_program_string(String program_string);

void setup()
{
	Serial.begin(115200);
	timer = Timer::getInstance();

	hw_timer = timerBegin(0, 80, true);
	timerAlarmWrite(hw_timer, 1000000, true);
	timerAttachInterrupt(hw_timer, &Timer::on_timer, true);
	timer->start_timer(hw_timer);

	display = new Display();
	clock_69 = new Clock(display);
	menu = new Menu();

	pinMode(IR_RECEIVER_PIN, INPUT);
	ir = new IR_Signal();
	ir->setup_ir();

	rtc = new RTC();

	if (!rtc->is_valid())
	{
		// set time
		timer->set_seconds_counter(23, 59, 50);
	}

	state = Function_State::IDLE;
}

void checkTime()
{
	if (timer == nullptr || clock_69 == nullptr || timer->display_queue == nullptr)
	{
		// throw exception or handle error
		Serial.println("Timer or Clock is uninitialized");
		return;
	}
	bool update_display = false;
	BaseType_t xStatus = xQueueReceive(timer->display_queue, (void *)&update_display, x_ticks_to_wait);
	if (xStatus == pdPASS)
	{
		// Serial.println("checking time");
		clock_69->update_clock_display(timer->seconds_counter);
	}
	else
	{
		// Serial.println("Unable to receive from queue");
	}
}

void menu_navigation(uint16_t ir_command, struct Program::prog_params *prog_params, Program **selected_program)
{
	switch (ir_command)
	{
	case IR_UP:
		display->clear_display();
		program_index++;
		if (program_index >= program_max)
			program_index = 0;
		break;

	case IR_DOWN:
		display->clear_display();
		program_index--;
		if (program_index > program_max - 1)
			program_index = program_max - 1;
		break;

	case IR_OK:
		display->clear_display();
		*selected_program = menu->select_program(program_index);
		*prog_params = (*selected_program)->get_prog_params();
		state = Function_State::CONFIGURING_PROGRAM;
		Serial.println((*selected_program)->get_name());
		break;

	case IR_BACK:
		state = Function_State::IDLE;
		display->clear_display();
		break;
	}
}

void loop()
{
	static struct Program::prog_params prog_params;
	static struct Program::program_display_info *program_display_info;
	static Program *selected_program = nullptr;
	uint16_t *ir_command = ir->get_ir_command();

	static uint8_t num_rounds_input;
	static uint16_t num_rest_seconds;
	static uint16_t num_work_seconds;

	switch (state)
	{
	case Function_State::IDLE:
		// Serial.println("State: IDLE");
		checkTime();
		if (ir_command != nullptr)
		{
			if (*ir_command == IR_UP || *ir_command == IR_DOWN)
			{
				Serial.printf("Receiving: %d, changing state to NAVIGATING_MENU\n", *ir_command);
				state = Function_State::NAVIGATING_MENU;
				display->clear_display();
			}
		}
		break;

	case Function_State::NAVIGATING_MENU:
		// Serial.printf("State: NAVIGATING_MENU\n");
		// Serial.printf("get_program_string: %s\n", menu->get_program_string(program_index));
		print_program_string(menu->get_program_string(program_index));
		if (ir_command != nullptr)
		{
			menu_navigation(*ir_command, &prog_params, &selected_program);
		}
		break;

	case Function_State::CONFIGURING_PROGRAM:
	{
		static enum class config_state {
			s_round = 0,
			s_work_red,
			s_work_green
		} c_state = config_state::s_round;
		switch (c_state)
		{
		case config_state::s_round:
		{
			// Serial.println("State: CONFIGURING_PROGRAM");
			//  TODO: Move this into the program interface.
			if (!prog_params.need_rounds)
			{
				c_state = config_state::s_work_red;
				break;
			}

			num_rounds_input = control_round_input(ir_command);

			if (num_rounds_input < 0xFF)
			{
				selected_program->set_num_rounds(num_rounds_input);
				c_state = config_state::s_work_red;
			}
			break;
		}
		case config_state::s_work_red:
		{
			if (!prog_params.need_work)
			{
				c_state = config_state::s_work_green;
				break;
			}

			num_work_seconds = control_work_input(ir_command, CRGB::Red);

			if (num_work_seconds < 0xFFFF)
			{
				selected_program->set_work_seconds(num_work_seconds);
				c_state = config_state::s_work_green;
			}
			break;
		}
		case config_state::s_work_green:
		{
			if (!selected_program->get_prog_params().need_rest)
			{
				state = Function_State::READY_TO_START;
				break;
			}

			num_rest_seconds = control_work_input(ir_command, CRGB::Green);

			if (num_rest_seconds < 0xFFFF)
			{
				selected_program->set_rest_seconds(num_rest_seconds);
				state = Function_State::READY_TO_START;
			}
			break;
		}
		default:
			break;
		}
	}
	break;

	case Function_State::READY_TO_START:
		// Serial.println("State: READY_TO_START");
		display->clear_display();
		display->write_string("ready", 5, CRGB::Aqua);
		display->push_to_display();

		program_display_info = selected_program->get_display_info();
		if (ir_command != nullptr)
		{
			switch (*ir_command)
			{
				// TODO: add back etc
			case IR_OK:
				selected_program->start();
				state = Function_State::RUNNING_TIMER;
				Serial.printf("Rounds: %d\n", program_display_info->rounds_remaining);
				Serial.printf("Seconds Elapsed: %d\n", program_display_info->seconds_display_val);
				break;
			default:
				break;
			}
		}
		break;

	case Function_State::RUNNING_TIMER:
		// Serial.println("State: RUNNING_TIMER");
		display->clear_display();
		bool temp;
		if (xQueueReceive(timer->display_queue, (void *)&temp, x_ticks_to_wait))
		{
			selected_program->on_notify();
			if (program_display_info != nullptr && program_display_info->display_rounds)
			{
				display->update_display(5, program_display_info->rounds_remaining / 10, CRGB::Green);
				display->update_display(4, program_display_info->rounds_remaining % 10, CRGB::Green);
				Serial.printf("Rounds remaining: %d\n", program_display_info->rounds_remaining);
			}

			if (program_display_info != nullptr)
			{
				if (program_display_info->currently_working)
				{
					display->convert_to_display(program_display_info->seconds_display_val, CRGB::Red);
					// Serial.printf("Work: %d seconds remaining\n", program_display_info->seconds_display_val);
				}
				else
				{
					display->convert_to_display(program_display_info->seconds_display_val, CRGB::Green);
					// Serial.printf("Rest: %d seconds remaining\n", program_display_info->seconds_display_val);
				}
			}
			display->push_to_display();

			if (selected_program->get_program_finished())
			{
				// TODO: return to IDLE
			}
		}
		break;
	default:
		break;
	}
}

void print_program_string(const String program_string)
{
	display->write_string(program_string, program_string.length(), CRGB::Red);
	display->push_to_display();
}

uint16_t control_work_input(uint16_t *ir_command, CRGB colour, CRGB selected_colour)
{
	static uint8_t selected_digit = 3;
	static std::array<uint8_t, 4> digit_values = {0};

	display->clear_display();

	// Treat each display_value digit as a discrete 0-9, no rolling over into higher positions, thats annoying
	// Display selected digit values, which is the same element in the array.
	display->update_display(3, digit_values[3], (selected_digit == 3) ? selected_colour : colour);
	display->update_display(2, digit_values[2], (selected_digit == 2) ? selected_colour : colour);
	display->update_display(1, digit_values[1], (selected_digit == 1) ? selected_colour : colour);
	display->update_display(0, digit_values[0], (selected_digit == 0) ? selected_colour : colour);

	display->push_to_display();

	// Select digit to input to, blink the digit selected.

	// Select digits to increment/decrement
	if (ir_command == nullptr)
	{
		return 0xFFFF;
	}

	switch (*ir_command)
	{
	case IR_RIGHT:
		selected_digit = (selected_digit == 0) ? selected_digit = 3 : --selected_digit;
		break;
	case IR_LEFT:
		selected_digit = (selected_digit == 3) ? selected_digit = 0 : ++selected_digit;
		break;
	case IR_UP:
		digit_values[selected_digit] = (digit_values[selected_digit] == 9) ? digit_values[selected_digit] = 0 : ++digit_values[selected_digit];
		break;
	case IR_DOWN:
		digit_values[selected_digit] = (digit_values[selected_digit] == 0) ? digit_values[selected_digit] = 9 : --digit_values[selected_digit];
		break;
	case IR_BACK:
		state = Function_State::IDLE;
		break;
	case IR_OK:
	{
		int total_seconds;
		total_seconds = digit_values[0];
		total_seconds += digit_values[1] * 10;
		total_seconds += digit_values[2] * 60;
		total_seconds += digit_values[3] * 600;
		selected_digit = 3;
		digit_values = {0};
		return total_seconds;
	}
	default:
		break;
	}

	return 0xFFFF;
}

uint8_t control_round_input(uint16_t *ir_command)
{
	static uint8_t num_input = 0;
	display->clear_display();
	display->write_string("rnd", 3, CRGB::Green);

	display->update_display(0, num_input % 10, CRGB::Red);
	display->update_display(1, num_input / 10, CRGB::Red);
	display->push_to_display();
	if (ir_command == nullptr)
	{
		return 0xFF;
	}

	switch (*ir_command)
	{
	case IR_BACK:
		state = Function_State::IDLE;
		break;
	case IR_UP:
		num_input = (num_input < 99) ? ++num_input : num_input = 0;
		break;
	case IR_DOWN:
		num_input = (num_input > 0) ? --num_input : num_input = 99;
		break;
	case IR_OK:
		return num_input;
	default:
		break;
	}

	return 0xFF; // invalid to flag state machine to stay put
}