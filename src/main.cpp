#include <Arduino.h>
#include <IRremote.hpp>

#include "Timer.h"
#include "Display.h"
#include "Menu.h"
#include "Clock.h"
#include "Countdown_Round.h"

// IR defines and setup
#define IR_RECEIVER_PIN 13
IRData ir_input;
IRrecv irrecv(IR_RECEIVER_PIN);
QueueHandle_t IR_queue;

#define IR_NIL 0
#define IR_UP 1
#define IR_DOWN 2
#define IR_RIGHT 3
#define IR_LEFT 4
#define IR_OK 5
#define IR_BACK 6

#define NUM_PROGRAMS 4

enum class Function_State
{
	IDLE,
	NAVIGATING_MENU,
	CONFIGURING_PROGRAM,
	READY_TO_START,
	RUNNING_TIMER,
};

hw_timer_t *hw_timer = nullptr;
Timer *timer = nullptr;
Display *display = nullptr;
Menu *menu = nullptr;
Clock *clock_69 = nullptr;

Function_State state;

int control_work_input(CRGB colour);
uint8_t control_round_input();
void capture_ir_commands();
void print_program_string(std::string program_index);

void setup()
{
	pinMode(IR_RECEIVER_PIN, INPUT); // Set the IR receiver pin as input
	irrecv.enableIRIn();
	IR_queue = xQueueCreate(20, sizeof(uint16_t));

	Serial.begin(115200);
	display = new Display();
	clock_69 = new Clock(display);
	menu = new Menu();

	timer = Timer::getInstance();
	hw_timer = timerBegin(0, 80, true);
	timerAlarmWrite(hw_timer, 1000000, true);
	timerAttachInterrupt(hw_timer, &Timer::on_timer, true);
	timerAlarmEnable(hw_timer);

	state = Function_State::IDLE;
	// Init the clock with the GPS time, but for now... setting the inital time values to 10:45:15.
	timer->set_seconds_counter(23, 59, 50);
}

void loop()
{
	// check IR Queue:
	ir_input.command = IR_NIL;
	capture_ir_commands();
	xQueueReceive(IR_queue, &ir_input, 10);

	// check timer queue:
	volatile bool update_display;
	xQueueReceive(timer->display_queue, (void *)&update_display, portMAX_DELAY);
	struct Program::prog_params prog_params;
	struct Program::program_display_info program_display_info;

	uint8_t program_index = 0;
	std::string program_string;
	Program *selected_program = nullptr;

	timer->start_timer(hw_timer);
	switch (state)
	{
	case Function_State::IDLE:
		if (update_display)
		{
			// Change to RTC once ready
			clock_69->update_clock_display(timer->seconds_counter);
		}
		if (ir_input.command == IR_UP || ir_input.command == IR_DOWN)
		{
			state = Function_State::NAVIGATING_MENU;
		}
		break;
	case Function_State::NAVIGATING_MENU:
		switch (ir_input.command)
		{
		case IR_UP:
			program_index = (program_index == menu->programs.size()) ? program_index = 0 : program_index++;
			print_program_string(menu->get_program_string(program_index));
			break;

		case IR_DOWN:
			program_index = (program_index == 0) ? program_index = menu->programs.size() : program_index--;
			print_program_string(menu->get_program_string(program_index));
			break;

		case IR_OK:
			selected_program = menu->select_program(program_index);
			prog_params = selected_program->get_prog_params();
			state = Function_State::CONFIGURING_PROGRAM;
		case IR_BACK:
			state = Function_State::IDLE;
			display->clear_display();
			break;
		default:
			break;
		}
		break;
	case Function_State::CONFIGURING_PROGRAM:
		if (prog_params.need_rounds)
		{
			uint8_t num_rounds_input = control_round_input();
			selected_program->set_num_rounds(num_rounds_input);
		}
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
		break;
	case Function_State::READY_TO_START:
		program_display_info = selected_program->get_display_info();
		selected_program->start();
		switch (ir_input.command)
		{
		case IR_OK:
			state = Function_State::RUNNING_TIMER;
			break;
		default:
			break;
		}
		break;
	case Function_State::RUNNING_TIMER:
		timer->start_timer(hw_timer);
		if (xQueueReceive(timer->display_queue, (void *)&update_display, portMAX_DELAY))
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

			if (selected_program->tick())
			{
				timer->stop_timer(hw_timer);
			}
		}
		break;
	default:
		break;
	}
}

void print_program_string(std::string program_string)
{
	display->write_string(program_string, program_string.length(), CRGB::Red);
	display->push_to_display();
}

void capture_ir_commands()
{
	IRData queue_ir_input;
	if (IrReceiver.decode())
	{
		queue_ir_input = IrReceiver.decodedIRData;
		xQueueSend(IR_queue, &(queue_ir_input.command), 0);
	}
	IrReceiver.resume();
}

int control_work_input(CRGB colour)
{
	uint8_t selected_digit = 3;
	std::array<uint8_t, 4> digit_values = {0};

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
		ir_input.command = IR_NIL;
		capture_ir_commands();
		xQueueReceive(IR_queue, &ir_input, 10);
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

uint8_t control_round_input()
{
	uint8_t num_input = 0;

	ir_input.command = IR_NIL;
	display->clear_display();

	display->write_string("rnd", 3, CRGB::Green);

	// TODO: blink the RED numbers?
	display->update_display(1, num_input / 10, CRGB::Red);
	display->update_display(0, num_input % 10, CRGB::Red);
	display->push_to_display();

	while (ir_input.command != IR_OK)
	{
		ir_input.command = IR_NIL;
		capture_ir_commands();
		xQueueReceive(IR_queue, &ir_input, 10);
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
		default:
			break;
		}

		display->update_display(0, num_input % 10, CRGB::Red);
		display->update_display(1, num_input / 10, CRGB::Red);
		display->push_to_display();
	}
	return num_input;
}