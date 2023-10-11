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

    uint8_t ten_second_countdown = 10;
    uint16_t seconds_to_work = 0;
    uint16_t seconds_to_rest = 0;
    uint8_t total_num_rounds = 0;

    uint16_t elapsed_seconds = 0;
    
    Phase program_phase;
    bool finished_program;

public:
    struct prog_params
    {
        // Set all to true, and instantiations should update to false if not needeed upon construction
        bool need_rounds = true;
        bool need_work = true;
        bool need_rest = true;
    };

    struct program_display_info
    {
        bool beep = false;
        bool display_rounds = false;
        bool currently_working = false;
        bool paused = false; // If program is paused, do something.

        uint8_t rounds_remaining = 0;
        uint16_t seconds_display_val = 0;
        
        uint16_t beep_milliseconds = 0;
    };

    virtual void set_prog_params() = 0;
    virtual void init_display_info() = 0;
    virtual struct prog_params get_prog_params() { return this->program_params; }
    virtual struct program_display_info* get_display_info() { return &this->program_display_info; }
    
    virtual void start()
    {
        this->program_phase = Phase::TEN_SECOND_TO_START;
        this->finished_program = false;
    }

    virtual void on_notify() = 0;

    const String get_name() { return program_name; }
    enum Phase get_program_phase() { return program_phase; }
    bool get_program_finished() { return finished_program; }
    void set_program_phase(enum Phase phase_) { this->program_phase = phase_; }

    Program(String program_name_)
    {
        Serial.printf("Setting program name to: %s\n", program_name_);
        this->program_name = program_name_;
    }

    virtual ~Program() = default;

    void reset_program()
    {
        seconds_to_work = 0;
        seconds_to_rest = 0;
        total_num_rounds = 0;
    }

    struct prog_params program_params;
    struct program_display_info program_display_info;

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
        this->program_display_info.rounds_remaining = num_rounds_;
        //Serial.printf("num rounds_: %d\n", num_rounds_);
        //Serial.printf("num rounds: %d\n", total_num_rounds);
    }
};
