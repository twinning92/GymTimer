#pragma once
#include "Program.h"

class Countup_Round : public Program
{
public:
	virtual bool show_rounds() final { return true; }	// Show the rounds counter
	virtual bool need_rounds() final { return true; }	// Rounds count is configured
	virtual bool need_work()   final { return true; }	// Requires a workout time
	virtual bool need_rest()   final { return false; }	// Does not use a rest time
	virtual bool direction()   final { return true; }	// counts up
	virtual bool has_target()  final { return true; }	// Stop program on round target
	
	Countup_Round() : Program("up_rnd") { }

	virtual void on_timer() final {
		Program::on_timer();
	}

	virtual enum User_Input_Action status() final {
		return Program::status();
	}
};
