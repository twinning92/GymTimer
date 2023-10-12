#pragma once
#include <Arduino.h>
#include "Observer.h"

enum class Phase
{
    TEN_SECOND_TO_START,
    WORK,
    REST,
    PAUSED,
    FINISHED,
};

class Program
{
protected:
    String program_name;

    uint16_t seconds_to_work = 0;
    uint16_t seconds_to_rest = 0;
    uint8_t total_num_rounds = 0;

public:
    struct prog_params
    {
        // Set all to true, and instantiations should update to false if not needeed upon construction
        bool need_rounds = true;
        bool need_work = true;
        bool need_rest = true;
    };

    struct program_runner
    {
        Phase program_phase;

        bool beep = false;
        bool show_rounds = false;
        bool currently_working = false;
        bool finished_program = false;

        bool paused = false; // If program is paused, do something.

        int8_t ten_second_countdown = 10;
        int8_t total_rounds = 0;       // Store number of rounds. Don't devrement this one. In case we want to reuse this?
        int16_t total_work_time = 0;
        int16_t total_rest_time = 0;

        int16_t seconds_value = 0; // Use seconds value to increment and calculate off. This is what is sent to the display.
        int8_t rounds_value = 0;   // Rounds value to actually display
        int16_t beep_milliseconds = 0;
    };

    virtual void set_prog_params() = 0;
    virtual void init_display_info() = 0;

    virtual struct program_runner *get_program_runner() { return &this->program_runner; }

    virtual void start()
    {
        this->program_runner.program_phase = Phase::TEN_SECOND_TO_START;
        this->program_runner.seconds_value = this->program_runner.ten_second_countdown;
        this->program_runner.total_rounds = this->total_num_rounds;
        this->program_runner.rounds_value = this->total_num_rounds;

        this->program_runner.total_work_time = this->seconds_to_work;
        this->program_runner.total_rest_time = this->seconds_to_rest;
    }

    virtual void on_notify() = 0;

    const String get_name() { return program_name; }

    Program(String program_name_)
    {
        Serial.printf("Setting program name to: %s\n", program_name_);
        this->program_name = program_name_;
    }

    virtual ~Program() = default;

    struct prog_params program_params;
    struct program_runner program_runner;

    virtual void set_work_seconds(uint16_t work_seconds_)
    {
        this->seconds_to_work = work_seconds_;
    }
    virtual void set_rest_seconds(uint16_t rest_seconds_)
    {
        this->seconds_to_rest = rest_seconds_;
    }
    virtual void set_num_rounds(uint8_t num_rounds_)
    {
        this->total_num_rounds = num_rounds_;
    }
};
