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
TickType_t x_ticks_to_wait = pdMS_TO_TICKS(0.5);
Timer *timer = nullptr;
Display *display = nullptr;
Menu *menu = nullptr;
Clock *clock_69 = nullptr;
RTC *rtc = nullptr;
IR_Signal *ir = nullptr;
uint8_t program_index = 0;
#define program_max (menu->programs.size() - 1)

Function_State state;

int control_work_input(CRGB colour);
uint8_t control_round_input();
void capture_ir_commands();
void print_program_string(String program_index);

void setup()
{
	Serial.begin(115200);

	display = new Display();
	clock_69 = new Clock(display);
	menu = new Menu();

	pinMode(IR_RECEIVER_PIN, INPUT); // Set the IR receiver pin as input

	timer = Timer::getInstance();
	hw_timer = timerBegin(0, 80, true);
	timerAlarmWrite(hw_timer, 1000000, true);
	timerAttachInterrupt(hw_timer, &Timer::on_timer, true);
	timerAlarmEnable(hw_timer);

	ir = new IR_Signal();
	ir->setup_ir();

	rtc = new RTC();

	if (!rtc->is_valid())
	{
		// set time
		timer->set_seconds_counter(23, 59, 50);
	}

	state = Function_State::IDLE;
	// Init the clock with the GPS time, but for now... setting the inital time values to 10:45:15.

	timer->start_timer(hw_timer);
}

void checkTime(void) {
	bool update_display;
	xQueueReceive(timer->display_queue, (void *)&update_display, x_ticks_to_wait);
	if (update_display)
	{
		// Change to RTC once ready
		clock_69->update_clock_display(timer->seconds_counter);
	}
}

void menu_navigation(IRData ir_input, struct Program::prog_params *prog_params, Program **selected_program) {
		switch (ir_input.command)
		{
		case IR_UP:
			display->clear_display();
			program_index++;
			if(program_index >= program_max)
				program_index = 0;
			break;

		case IR_DOWN:
			display->clear_display();
			program_index--;
			if (program_index < 0)
				program_index = program_max;
			break;

		case IR_OK:
			display->clear_display();
			*selected_program = menu->select_program(program_index);
			*prog_params = menu->select_program(program_index)->get_prog_params();
			state = Function_State::CONFIGURING_PROGRAM;
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
	static struct Program::program_display_info program_display_info;
	static Program *selected_program = nullptr;


	// check IR Queue: 
	// TODO: just return the received code - no queue no point as not ISR context
	ir->enqueue_ir_commands();
	IRData ir_input = ir->get_from_queue();
	// TODO: change to return pointer and if NULL then no command received


	switch (state)
	{
	case Function_State::IDLE:
		Serial.println("State: IDLE");
		checkTime();
		// check timer queue:
		if (ir_input.command == IR_UP || ir_input.command == IR_DOWN)
		{
			Serial.printf("Receiving: %d, changing state to NAVIGATING_MENU\n", ir_input.command);
			state = Function_State::NAVIGATING_MENU;
			display->clear_display();
		}
		break;


	case Function_State::NAVIGATING_MENU:
		Serial.printf("State: NAVIGATING_MENU\n");
		Serial.printf("get_program_string: %s\n", menu->get_program_string(program_index));
		print_program_string(menu->get_program_string(program_index));
		menu_navigation(ir_input, &prog_params, &selected_program);
		break;



	case Function_State::CONFIGURING_PROGRAM: {
		static enum class config_state{
			s_round = 0,
			s_work_red,
			s_work_green
		} c_state = config_state::s_round;
		switch(c_state) {
			case config_state::s_round:
				//Serial.println("State: CONFIGURING_PROGRAM");
				// TODO: Move this into the program interface.
				if (!prog_params.need_rounds) {
					c_state = config_state::s_work_red;
					break;
				}
				uint8_t num_rounds_input = control_round_input(ir_input);
				if(num_rounds_input < 0xFF) {
					selected_program->set_num_rounds(num_rounds_input);
					c_state = config_state::s_work_red;
				}
			}




	// TODO: others....

			if (prog_params.need_work)
			{
				uint8_t work_seconds_input = control_work_input(CRGB::Red);
				selected_program->set_work_seconds(work_seconds_input);
			}
			if (prog_params.need_rest)
			{
				uint8_t rest_seconds_input = control_work_input(CRGB::Green);
				selected_program->set_rest_seconds(rest_seconds_input);
			}
			if (!prog_params.need_rounds && !prog_params.need_work && !prog_params.need_rest)
			{
				state = Function_State::READY_TO_START;
			}
	}
		break;
	
	case Function_State::READY_TO_START:
		Serial.println("State: READY_TO_START");
		program_display_info = selected_program->get_display_info();
		switch (ir_input.command)
		{
			// TODO: add back etc
		case IR_OK:
			selected_program->start();
			state = Function_State::RUNNING_TIMER;
			break;
		default:
			break;
		}
		break;

		
	case Function_State::RUNNING_TIMER:
		Serial.println("State: RUNNING_TIMER");
		bool temp;
		if (xQueueReceive(timer->display_queue, (void *)&temp, portMAX_DELAY))
		{
			if (program_display_info.display_rounds)
			{
				display->update_display(5, program_display_info.rounds_remaining / 10, CRGB::Green);
				display->update_display(4, program_display_info.rounds_remaining % 10, CRGB::Green);
			}

			if (program_display_info.currently_working)
			{
				display->convert_to_display(program_display_info.seconds_value, CRGB::Red);
			}
			else
			{
				display->convert_to_display(program_display_info.seconds_value, CRGB::Green);
			}

			display->push_to_display();

			if (selected_program->get_program_finished())
			{
				//TODO: return to IDLE
			}
		}
		break;
	default:
		break;
	}
}

void print_program_string(const String program_string)
{
	display->write_string(program_string, program_string.length() - 1, CRGB::Red);
	display->push_to_display();
}

int control_work_input(CRGB colour)
{
	uint8_t selected_digit = 3;
	std::array<uint8_t, 4> digit_values = {0};
	IRData ir_input;
	ir_input.command = IR_NIL;

	display->clear_display();

	// Treat each display_value digit as a discrete 0-9, no rolling over, thats annoying
	// Display selected digit values, which is the same element in the array.
	display->update_display(3, digit_values[selected_digit], colour);
	display->update_display(2, digit_values[selected_digit], colour);
	display->update_display(1, digit_values[selected_digit], colour);
	display->update_display(0, digit_values[selected_digit], colour);

	// Select digit to input to, blink the digit selected.
	while (ir_input.command != IR_OK)
	{
		ir->enqueue_ir_commands();
		ir_input = ir->get_from_queue();
		// Select digits to increment/decrement
		switch (ir_input.command)
		{
		case IR_RIGHT:
			selected_digit = (selected_digit == 0) ? selected_digit = 3 : selected_digit--;
			break;
		case IR_LEFT:
			selected_digit = (selected_digit == 3) ? selected_digit = 0 : selected_digit++;
			break;
		case IR_UP:
			digit_values[selected_digit] = (digit_values[selected_digit] == 9) ? digit_values[selected_digit] = 0 : digit_values[selected_digit]++;
			break;
		case IR_DOWN:
			digit_values[selected_digit] = (digit_values[selected_digit] == 0) ? digit_values[selected_digit] = 9 : digit_values[selected_digit]--;
			break;
		case IR_BACK:
			state = Function_State::IDLE;
			break;
		default:
			break;
		}

		display->update_display(selected_digit, digit_values[selected_digit]);
		display->push_to_display();
	}

	int total_seconds;
	total_seconds = digit_values[0];
	total_seconds += digit_values[1] * 10;
	total_seconds += digit_values[2] * 60;
	total_seconds += digit_values[3] * 600;

	return total_seconds;
}

uint8_t control_round_input(IRData ir_input)
{
	static uint8_t num_input = 0;
	display->clear_display();
	display->write_string("rnd", 3, CRGB::Green);

	switch (ir_input.command)
	{
		case IR_BACK:
			state = Function_State::IDLE;
			break;
		case IR_UP:
			num_input = (num_input < 99) ? num_input++ : num_input = 0;
			break;
		case IR_DOWN:
			num_input = (num_input > 0) ? num_input-- : num_input = 99;
			break;
		case IR_OK:
			return num_input;
		default:
			break;
	}

	display->update_display(0, num_input % 10, CRGB::Red);
	display->update_display(1, num_input / 10, CRGB::Red);
	display->push_to_display();
	return 0xFF; // invalid to flag state machine to stay put
}