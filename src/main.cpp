#include <Arduino.h>
#include <IRremote.hpp>

#include "Timer.h"
#include "Display.h"
#include "Menu.h"
#include "Clock.h"
#include "Countdown_Round.h"

// IR defines and setup
#define IR_RECEIVER_PIN 13
decode_results ir_input;
IRrecv irrecv(IR_RECEIVER_PIN);
QueueHandle_t IR_queue;

#define IR_NULL 0
#define IR_UP 1
#define IR_DOWN 2
#define IR_RIGHT 3
#define IR_LEFT 4
#define IR_OK 5
#define IR_BACK 6

#define NUM_PROGRAMS 4

hw_timer_t *hw_timer = nullptr;
Timer *timer;
Display *display;
Menu *menu;
Clock *clock_69;

enum class Function_State
{
	IDLE,
	NAVIGATING_MENU,
	CONFIGURING_PROGRAM,
	READY_TO_START,
	RUNNING_TIMER,
};

Function_State state = Function_State::IDLE;

uint8_t program_index = 0;
std::string program_string;
Program *selected_program = nullptr;
struct Program::prog_params prog_params;

int control_work_input();
uint8_t control_round_input();

void setup()
{
	pinMode(IR_RECEIVER_PIN, INPUT); // Set the IR receiver pin as input
	irrecv.enableIRIn();
	IR_queue = xQueueCreate(20, sizeof(decode_results));

	Serial.begin(115200);
	display = new Display();
	clock_69 = new Clock(display);
	menu = new Menu();

	timer = Timer::getInstance();
	hw_timer = timerBegin(0, 80, true);
	timerAlarmWrite(hw_timer, 1000000, true);
	timerAttachInterrupt(hw_timer, &Timer::on_timer, true);
	timerAlarmEnable(hw_timer);

	// Init the clock with the GPS time, but for now... setting the inital time values to 10:45:15.
	timer->set_seconds_counter(23, 59, 50);
}

void loop()
{
	// check IR Queue:
	ir_input.value = IR_NULL;
	capture_ir_commands();
	xQueueReceive(IR_queue, &ir_input, 10);

	// check timer queue:
	volatile bool update_display;
	xQueueReceive(timer->display_queue, (void *)&update_display, portMAX_DELAY);

	switch (state)
	{
	case Function_State::IDLE:
		clock_69->update_clock_display(timer->seconds_counter);
		if (ir_input.value == IR_UP || ir_input.value == IR_DOWN)
		{
			state = Function_State::NAVIGATING_MENU;
		}
		break;
	case Function_State::NAVIGATING_MENU:
		switch (ir_input.value)
		{
		case IR_UP:
			program_index == menu->programs.size() ? program_index = 0 : program_index++;
			print_program_string(program_index);
			break;

		case IR_DOWN:
			program_index == 0 ? program_index = menu->programs.size() : program_index--;
			print_program_string(program_index);
			break;

		case IR_OK:
			selected_program = menu->select_program(program_index);
			struct Program::prog_params prog_params = selected_program->get_prog_params();
			state = Function_State::CONFIGURING_PROGRAM;
			
		case IR_NULL:
		default:
			break;
		}
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
		struct Program::program_display_info program_display_info = selected_program->get_display_info();
		selected_program->start();
		switch (ir_input.value)
		{
		case IR_OK:
			state = Function_State::RUNNING_TIMER;
			break;
		default:
			break;
		}
		break;
	case Function_State::RUNNING_TIMER:
		volatile bool display_update_queued;

		timer->start_timer(hw_timer);
		if (xQueueReceive(timer->display_queue, (void *)&display_update_queued, portMAX_DELAY))
		{
			display->convert_to_display(selected_program->tick());
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

void print_program_string(uint8_t program_index)
{
	program_string = menu->get_program_string(program_index);
	display->write_string(program_string, program_string.length(), CRGB::Red);
	display->push_to_display();
}

void capture_ir_commands()
{
	decode_results queue_ir_input;
	if (irrecv.decode(&queue_ir_input))
	{
		xQueueSend(IR_queue, &queue_ir_input, 0);
	}
	irrecv.resume();
}

int control_work_input(CRGB colour)
{

	uint8_t selected_digit = 3;
	std::array<uint8_t, 4> digit_values = {0};

	ir_input.value = IR_NULL;
	display->clear_display();

	// Treat each display_value digit as a discrete 0-9, no rolling over, thats annoying
	// Display selected digit values, which is the same element in the array.
	display->update_display(3, digit_values[selected_digit], colour);
	display->update_display(2, digit_values[selected_digit], colour);
	display->update_display(1, digit_values[selected_digit], colour);
	display->update_display(0, digit_values[selected_digit], colour);

	// Select digit to input to, blink the digit selected.
	while (ir_input.value != IR_OK)
	{
		ir_input.value = IR_NULL;
		capture_ir_commands();
		xQueueReceive(IR_queue, &ir_input, 10);
		// Select digits to increment/decrement
		switch (ir_input.value)
		{
		case IR_RIGHT:
			selected_digit == 0 ? selected_digit = 3 : selected_digit--;
			break;
		case IR_LEFT:
			selected_digit == 3 ? selected_digit = 0 : selected_digit++;
			break;
		case IR_UP:
			digit_values[selected_digit] == 9 ? digit_values[selected_digit] = 0 : digit_values[selected_digit]++;
			break;
		case IR_DOWN:
			digit_values[selected_digit] == 0 ? digit_values[selected_digit] = 9 : digit_values[selected_digit]--;
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

	ir_input.value = IR_NULL;
	display->clear_display();

	display->write_string("rnd", 3, CRGB::Green);

	// TODO: blink the RED numbers?
	display->update_display(1, num_input / 10, CRGB::Red);
	display->update_display(0, num_input % 10, CRGB::Red);
	display->push_to_display();

	while (ir_input.value != IR_OK)
	{
		ir_input.value = IR_NULL;
		capture_ir_commands();
		xQueueReceive(IR_queue, &ir_input, 10);
		switch (ir_input.value)
		{
		case IR_BACK:
			state = Function_State::IDLE;
			break;
		case IR_UP:
			num_input < 99 ? num_input++ : num_input = 0;
			break;
		case IR_DOWN:
			num_input > 0 ? num_input-- : num_input = 99;
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