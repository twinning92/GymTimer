#pragma once
#include <Arduino.h>

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
    std::string program_name;
    uint8_t start_seconds = 10;
    uint16_t work_seconds;
    uint16_t rest_seconds;
    uint8_t num_rounds;
    Phase program_phase;
    uint16_t elapsed_seconds = 0;

public:
    struct prog_params
    {
        // Set all to true, and instantiations should update to false if not needeed upon construction
        bool need_rounds = true;
        bool need_rest = true;
        bool need_work = true;
    };

    struct program_display_info
    {
        bool beep = false;
        bool display_rounds;
        bool currently_working = false;
        bool paused = false; // If program is paused, do something.
        uint8_t rounds_remaining;

        uint16_t beep_milliseconds;

        uint16_t seconds_value;
    };
    virtual void set_prog_params() = 0;
    virtual void init_display_info() = 0;
    virtual struct prog_params get_prog_params() { return this->program_params; }
    virtual struct program_display_info get_display_info() { return this->program_display_info; }
    virtual void start() { this->program_phase = Phase::TEN_SECOND_TO_START; };
    virtual bool tick() = 0;

    const std::string get_name() { return program_name; };
    enum Phase get_program_phase() { return program_phase; }
    void set_program_phase(enum Phase phase_) { this->program_phase = phase_; }

    Program(std::string program_name_)
    {
        this->program_name = program_name_;
        this->set_prog_params();
        this->init_display_info();
    }

    virtual ~Program() = default;

    void reset_program()
    {
        work_seconds = 0;
        rest_seconds = 0;
        num_rounds = 0;
    }

    struct prog_params program_params;
    struct program_display_info program_display_info;

    virtual void set_work_seconds(int work_seconds_)
    {
        work_seconds = work_seconds_;
        this->program_params.need_work = false;
    }
    virtual void set_rest_seconds(int rest_seconds_)
    {
        rest_seconds = rest_seconds_;
        this->program_params.need_rest = false;
    }
    virtual void set_num_rounds(int num_rounds_)
    {
        num_rounds = num_rounds_;
        this->program_params.need_rounds = false;
    }
};
