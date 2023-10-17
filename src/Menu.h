#pragma once

#include "config.h"
// used in the below includes so needs to be declared here
enum User_Input_Action {
	IA_ACCEPT, IA_BUSY, IA_BACK
};

#include "IR_Signal.h"
#include "Display.h"
#include "RTC.h"
#include "Buzzer.h"
#include "Program.h"
#include "programs/Countdown_Round.h"
#include "programs/Jits.h"
#include "programs/Countup_Round.h"

#define NUM_PROGRAMS 5




class Menu
{
private:
	enum Function_State
	{
		FS_IDLE,
		FS_NAVIGATING_MENU,
		FS_CONFIGURING_PROGRAM,
		FS_READY_TO_START,
		FS_RUNNING_TIMER,
	};
	Function_State state = FS_IDLE;
	Program *prog_list[NUM_PROGRAMS];
	uint8_t prog_idx = 0;

public:
	Menu() : prog_list{
				 new Countdown_Round(),
				 new Countup_Round(),
				 new Jits(3),
				 new Jits(5),
				 new Jits(7),
			 }
	{
	}

	void reset()
	{
		state = FS_IDLE;
		for(uint8_t idx = 0; idx < NUM_PROGRAMS; idx++) // reset all programs
			prog_list[prog_idx]->reset();
		prog_idx = 0;
	}

	// called from main 100msec timer
	void on_timer() {
		if(state == FS_RUNNING_TIMER)
			prog_list[prog_idx]->on_timer();
	}

	String state_name() {
		switch (state) {
			case FS_IDLE: 					return "FS_IDLE";
			case FS_NAVIGATING_MENU: 		return "FS_NAVIGATING_MENU";
			case FS_CONFIGURING_PROGRAM: 	return "FS_CONFIGURING_PROGRAM";
			case FS_READY_TO_START: 		return "FS_READY_TO_START";
			case FS_RUNNING_TIMER: 			return "FS_RUNNING_TIMER";
		}
		return "Unknown";
	}

	// called from main loop
	void tick()
	{
		D_PRINT(8, "menu state: %s\n", state_name());
		switch (state)
		{
		case FS_IDLE:
			state_idle();
			break;

		case FS_NAVIGATING_MENU:
			state_nav();
			break;

		case FS_CONFIGURING_PROGRAM:
			state_config();
			break;

		case FS_READY_TO_START:
			state_ready();
			break;

		case FS_RUNNING_TIMER:
			state_running();
			break;
		default:
			break;
		}
	}
private:

	User_Input_Action input_4dig(uint16_t *value, CRGB colour, CRGB highlight = CRGB::White)
	{
		static uint8_t selected_digit = 3;
		const uint16_t incdec_vals[4] = {600, 60, 10, 1};

		display.clear_display();
		display.show_time(*value, selected_digit, highlight, colour);
		display.push_to_display();

		switch (ir.get_ir_command())
		{
			case IR_RIGHT:
				selected_digit--;
				selected_digit &= 0x03; // simple bitmask makes it 0-3
				buzzer.start(2);
				break;
			case IR_LEFT:
				selected_digit++;
				selected_digit &= 0x03; // simple bitmask makes it 0-3
				buzzer.start(2);
				break;
			case IR_UP:
				*value += incdec_vals[selected_digit];
				if(*value >= (3600)) // roll over check
					*value -= 3600; // roll it back
				buzzer.start(2);
				break;
			case IR_DOWN:
				*value -= incdec_vals[selected_digit];
				if(*value >= 3600) // roll under check
					*value += 3600; // roll it back
				buzzer.start(2);
				break;
			case IR_BACK:
				buzzer.start(2);
				return IA_BACK;
			case IR_OK:
				buzzer.start(2);
				selected_digit = 3;
				return IA_ACCEPT;
			default:
				break;
		}
		return IA_BUSY;
	}

	/**
	 * @brief accept 2 digit user input
	 * 
	 * @param num_input 			pointer to value to alter
	 * @return User_Input_Action 	action result
	 */
	User_Input_Action input_2dig(uint8_t *num_input)
	{
		display.write_string("rnd   ", 6, CRGB::Green);
		display.set_digit(1, *num_input / 10, CRGB::Red);
		display.set_digit(0, *num_input % 10, CRGB::Red);
		display.push_to_display();
		switch (ir.get_ir_command())
		{
			case IR_BACK:
				buzzer.start(2);
				return IA_BACK;
			case IR_UP:
				buzzer.start(2);
				*num_input = *num_input < 99 ? *num_input + 1 : 0;
				break;
			case IR_DOWN:
				buzzer.start(2);
				*num_input = *num_input > 0 ? *num_input - 1 : 99;
				break;
			case IR_OK:
				buzzer.start(2);
				return IA_ACCEPT;
			default:
				break;
		}
		return IA_BUSY;
	}

	void state_idle()
	{
		// Display current time by default
		String clk = "------";
		RtcDateTime now = Rtc.get();
		clk[5] = now.Hour() / 10;
		clk[4] = now.Hour() % 10;
		clk[3] = now.Minute() / 10;
		clk[2] = now.Minute() % 10;
		clk[1] = now.Second() / 10;
		clk[0] = now.Second() % 10;
		display.write_string(clk, 6, CRGB::Honeydew);
		display.push_to_display();
		// check for IR commands
		switch (ir.get_ir_command())
		{
		case IR_UP:
			prog_idx = 0;
			state = FS_NAVIGATING_MENU;
			buzzer.start(2);
			break;

		case IR_DOWN:
			prog_idx = NUM_PROGRAMS - 1;
			state = FS_NAVIGATING_MENU;
			buzzer.start(2);
			break;
		}
	}
	void state_nav()
	{
		// Display current program name
		display.write_string(prog_list[prog_idx]->name(), 6, CRGB::Honeydew);
		display.push_to_display();
		// check for IR commands
		switch (ir.get_ir_command())
		{
		case IR_UP:
			prog_idx++;
			if (prog_idx >= NUM_PROGRAMS)
				prog_idx = 0;
			buzzer.start(2);
			break;

		case IR_DOWN:
			prog_idx--;
			if (prog_idx > NUM_PROGRAMS - 1)
				prog_idx = NUM_PROGRAMS - 1;
			buzzer.start(2);
			break;

		case IR_OK:
			prog_list[prog_idx]->reset();
			state = FS_CONFIGURING_PROGRAM;
			buzzer.start(2);
			break;

		case IR_BACK:
			state = FS_IDLE;
			buzzer.start(2);
			break;
		}
	}
	void state_config()
	{
		static uint8_t input_rounds;
		static uint16_t input_work;
		static uint16_t input_rest;
		
		static enum config_state { CS_INIT, CS_ROUND, CS_WORK, CS_REST, CS_DONE, } c_state = CS_INIT;
		Program *current_prog = prog_list[prog_idx];
		switch (c_state)
		{
			// When we first come in here we need to update our selecion values from the program to alter them
			case CS_INIT:
				input_rounds = current_prog->get_rounds();
				input_work = current_prog->get_work_time();
				input_rest = current_prog->get_rest_time();
				c_state = CS_ROUND;
				break;
				
		case CS_ROUND:
			if (!current_prog->need_rounds())
			{
				c_state = CS_WORK;
				break;
			}
			switch(input_2dig(&input_rounds)) {
				case IA_BUSY: // busy
					break;
				case IA_BACK: // back
					state = FS_NAVIGATING_MENU;
					break;
				case IA_ACCEPT:
					current_prog->set_rounds(input_rounds);
					c_state = CS_WORK;
					break;
			}
			break;

		case CS_WORK:
			if (!current_prog->need_work())
			{
				c_state = CS_REST;
				break;
			}
			switch(input_4dig(&input_work, CRGB::Red)) {
				case IA_BUSY: // busy
					break;
				case IA_BACK: // back
					c_state = CS_ROUND;
					break;
				default:
					current_prog->set_work_time(input_work);
					c_state = CS_REST;
					break;
			}
			break;

		case P_REST:
			if (!current_prog->need_rest())
			{
				state = FS_READY_TO_START;
				break;
			}
			switch(input_4dig(&input_rest, CRGB::Green)) {
				case IA_BUSY: // busy
					break;
				case IA_BACK: // back
					c_state = CS_WORK;
					break;
				default:
					current_prog->set_rest_time(input_rest);
					state = FS_READY_TO_START;
					break;
			}
			break;

		default:
			break;
		}
	}
	void state_ready() {
		display.write_string("ready ", 6, CRGB::Aqua);
		display.push_to_display();
		Program *current_prog = prog_list[prog_idx];
		
		switch (ir.get_ir_command()) {
			case IR_BACK:
				state = FS_NAVIGATING_MENU;
				buzzer.start(2);
				break;
			case IR_OK:
				current_prog->start();
				state = FS_RUNNING_TIMER;
				buzzer.start(2);
				break;
			default:
				break;
		}
	}
	void state_running() 
	{
		Program *current_prog = prog_list[prog_idx];
		switch(current_prog->status()) {
			case IA_BUSY: // program is busy running
				break;
			case IA_ACCEPT: // program finished
				state = FS_IDLE;
				break;
			case IA_BACK: // program cancelled
				state = FS_READY_TO_START;
				break;
		}
	}
};

// Menu handler
extern Menu menu;
