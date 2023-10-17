#pragma once
#include "Arduino.h"
#include "Menu.h"
#include "IR_Signal.h"
#include "Buzzer.h"
#include "Display.h"

enum Phase
{
	P_IDLE,
	P_PREP,
	P_TEN_SECOND_TO_START,
	P_WORK,
	P_REST,
	P_PAUSED,
	P_FINISHED,
	P_CANCELLED,
};

// TODO: add beeps
class Program
{
protected:
	// Configuration
	String m_name;
	uint16_t m_work_time = 0;
	uint16_t m_rest_time = 0;
	uint8_t m_rounds = 0;
	// Runner variables
	Phase m_phase = P_IDLE;

	uint16_t m_current_work_time;
	uint16_t m_current_rest_time;
	uint8_t m_current_rounds;

	uint8_t m_initial_countdown = 10;

	uint16_t m_beep_milliseconds = 0;


public:
	// base constructor
	Program(String prog_name) { m_name = prog_name; }

	// Override these as needed in each program
	virtual bool show_rounds() { return false; } // Is the rounds counter shown while running?
	virtual bool direction() { return false; }	 // Direction to count rounds T = up
	virtual bool has_target() { return true; }	 // Stop program on round target?
	virtual bool need_rounds() { return true; }
	virtual bool need_work() { return true; }
	virtual bool need_rest() { return true; }

	// Return the program's name
	String name() const
	{
		return m_name;
	}

	// Set the workout time in seconds
	void set_work_time(uint16_t work_time)
	{
		if (need_work())
		{
			m_work_time = work_time;
		}
	}
	// Get the workout time in seconds
	uint16_t get_work_time()
	{
		return m_work_time;
	}

	// Set the resting time in seconds
	void set_rest_time(uint16_t rest_time)
	{
		if (need_rest())
		{
			m_rest_time = rest_time;
		}
	}
	// Get the resting time in seconds
	uint16_t get_rest_time()
	{
		return m_rest_time;
	}

	// Set the number of rounds
	void set_rounds(uint8_t rounds)
	{
		if (need_rounds())
		{
			m_rounds = rounds;
		}
	}
	// Get the number of rounds
	uint16_t get_rounds()
	{
		return m_rounds;
	}

	void start()
	{
		m_phase = P_PREP;
	}
	void reset()
	{
		m_phase = P_IDLE;
	}
	// called from the timer @ 100ms interval
	virtual void on_timer()
	{
		static uint8_t divider = 0;
		static Phase last_phase = P_IDLE;
		if (m_phase != P_PAUSED)
		{
			divider++;
		}
		IR_Command cmd = ir.get_ir_command();
		// pause/cancel?
		if (cmd == IR_OK)
		{
			if (m_phase == P_PAUSED)
			{
				m_phase = last_phase; // unpause
			}
			else
			{
				last_phase = m_phase; // pause
				m_phase = P_PAUSED;
			}
		}
		else if (cmd == IR_BACK)
		{
			if (m_phase == P_PAUSED)
			{
				m_phase = last_phase; // unpause
			}
			else
			{
				m_phase = P_CANCELLED; // cancel
			}
		}
		// every second - we do this so that even tho we are running with a 1 second counter we have 100msec latency
		if ((m_phase == P_PREP) || (divider == 10))
		{
			divider = 0;
			switch (m_phase)
			{
			case P_PREP:
				m_current_rounds = (!direction() && has_target()) ? m_rounds : 0;
				m_current_work_time = m_work_time;
				m_initial_countdown = 10;
				m_phase = P_TEN_SECOND_TO_START;
				break;

			case P_TEN_SECOND_TO_START:
				m_initial_countdown--;
				switch (m_initial_countdown)
				{
				case 3:
				case 2:
				case 1:
					buzzer.start(2); // beep for 200msec
					break;
				case 0:
					buzzer.start(5); // beep for 500msec
					m_phase = P_WORK;
					break;
				}
				break;

			case P_WORK:
				display.clear_display();
				if (show_rounds())
				{
					display.set_digit(5, m_current_rounds / 10, CRGB::Red);
					display.set_digit(4, m_current_rounds % 10, CRGB::Red);
				}
				display.show_time(m_current_work_time, CRGB::Red);
				display.push_to_display();
				if (m_current_work_time <= 1) // the last second
				{
					m_current_rest_time = m_rest_time;
					m_phase = P_REST;
				}
				m_current_work_time--;
				break;

			case P_REST:
				display.clear_display();
				if (show_rounds())
				{
					display.set_digit(5, m_current_rounds / 10, CRGB::Green);
					display.set_digit(4, m_current_rounds % 10, CRGB::Green);
				}
				display.show_time(m_current_work_time, CRGB::Green);
				display.push_to_display();
				if (m_current_rest_time <= 1) // the last second
				{
					m_phase = P_FINISHED; // default to finished the rounds
					if (direction()) // up/down
					{
						// counting up to target
						m_current_rounds++;
						if (!has_target() || m_current_rounds < m_rounds) // no rounds left or there is no target
						{
							m_current_work_time = m_work_time;
							m_phase = P_WORK;
						}
					}
					else
					{
						// counting down to zero
						m_current_rounds--;
						if (m_current_rounds) // still have rounds left
						{
							m_current_work_time = m_work_time;
							m_phase = P_WORK;
						}
					}
				}
				m_current_rest_time--;
				break;

			case P_PAUSED:
				break;

			case P_FINISHED:
				break;

			case P_CANCELLED:
				break;
			}
		}
	}

	virtual enum User_Input_Action status()
	{
		switch (m_phase)
		{
		case P_FINISHED:
			m_phase = P_IDLE;
			return IA_ACCEPT;
		case P_CANCELLED:
			m_phase = P_IDLE;
			return IA_BACK;
		default:
			break;
		}
		return IA_BUSY;
	}
};
