#pragma once

#include <RtcDS1302.h>

class RTC
{
public:
    RTC();
    bool is_valid();

private:
    ThreeWire myWire;
    RtcDS1302<ThreeWire> rtc;
    RtcDateTime date_time;
};