#include "RTC.h"

RTC::RTC() : myWire(4, 5, 2), rtc(myWire), date_time(RtcDateTime(__DATE__, __TIME__))
{
    rtc.Begin();

    if (rtc.GetIsWriteProtected())
    {
        rtc.SetIsWriteProtected(false);
    }

    if (!rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        rtc.SetIsRunning(true);
    }
}
bool RTC::is_valid()
{
    RtcDateTime now = rtc.GetDateTime();
    return rtc.IsDateTimeValid() && now < date_time;
}
