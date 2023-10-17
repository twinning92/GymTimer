#pragma once
#include "Program.h"

// The Jits program operates as follows:
// x minutes on, x minutes off
// Rounds are counted instead of decremented.

// There will be a Jits, 5 Jits, 7 Jits where 5/7 are the length of the work rounds.
class Jits : public Program
{
public:
	virtual bool show_rounds() final { return true; }	// Show the rounds counter
	virtual bool need_rounds() final { return false; }	// Rounds not configurable
	virtual bool need_work()   final { return false; }	// Workout time not configurable
	virtual bool need_rest()   final { return false; }	// Rest time not configurable
	virtual bool direction()   final { return true; }	// Count up
	virtual bool has_target()  final { return false; }	// Don't stop program

	Jits(uint8_t time) : Program("jits  ") {
		m_name[5] = '0' + time;
		m_work_time = time;
		m_rest_time = time;
	}

	virtual void on_timer() final {
		Program::on_timer();
	}

	virtual enum User_Input_Action status() final {
		return Program::status();
	}
};
