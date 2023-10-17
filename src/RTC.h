#pragma once

#include <RtcDS1302.h>

class RTC {
private:
    ThreeWire myWire;
    RtcDS1302<ThreeWire> rtc;
    RtcDateTime date_time;

public:
	RTC() : 
		myWire(4, 5, 2), 
		rtc(myWire), 
		date_time(RtcDateTime(__DATE__, __TIME__))
	{
		rtc.Begin();
		if (rtc.GetIsWriteProtected()) {
			rtc.SetIsWriteProtected(false);
		}
		if (!rtc.GetIsRunning()) {
			// Serial.println("RTC was not actively running, starting now");
			rtc.SetIsRunning(true);
		}
	}

	bool is_valid() 
	{
		RtcDateTime now = rtc.GetDateTime();
		return rtc.IsDateTimeValid() && now < date_time;
	}

	RtcDateTime get() {
		return rtc.GetDateTime();
	}
};

// Real Time Clock handler
extern RTC Rtc;
