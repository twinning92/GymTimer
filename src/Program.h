#pragma once
#include <cstring>

class Program
{
public:
    virtual void configure_program();
    virtual const char *get_name();
    virtual void set_work_seconds(int work_seconds);
    virtual void set_rest_seconds(int rest_seconds);
    virtual void set_num_rounds(int num_rounds);
    virtual void reset_program();

    Program(const char* in_program_name) { strncpy(program_name, in_program_name, 6); }
    ~Program() = default;

protected:
    char program_name[6];
    int work_seconds;
    int rest_seconds;
    int num_rounds;
};