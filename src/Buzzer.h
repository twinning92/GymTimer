#pragma once
#include <Arduino.h>
#include "config.h"


class Buzzer
{
private:
	// n * 100msec time to buzz
	uint8_t m_time = 0;

public:
	Buzzer()
	{
		pinMode(BUZZER_PIN, OUTPUT);
		digitalWrite(BUZZER_PIN, LOW);
	}

	// called by 100msec timer
	void on_timer()
	{
		if (m_time)
		{
			digitalWrite(BUZZER_PIN, HIGH);
			m_time--;
		}
		else
		{
			digitalWrite(BUZZER_PIN, LOW);
		}
	}

	// start the buzzer for n * 100msec
	void start(uint8_t time)
	{
		m_time = time;
	}
};

extern Buzzer buzzer;
