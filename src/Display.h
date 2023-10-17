#pragma once
#include <FastLED.h>

#define DATA_PIN 15

#define SEG_LED_CNT_H	(9)
#define SEG_LED_CNT_V	(8)
#define DIG_LED_CNT		(3 * SEG_LED_CNT_H + 4 * SEG_LED_CNT_V)
#define COL_LED_CNT		(4)
#define LED_COUNT		(6 * DIG_LED_CNT + 1 * COL_LED_CNT)

// static const means it lives globlly once in this file only
// mapping from 7-bit ascii to 7seg equivalent
static const uint8_t digit_segment_mapping[128] = {
	0,			0,			0,			0,			0,			0,			0,			0,			// char 00 to 07 [non printable]
	0,			0,			0,			0,			0,			0,			0,			0,			// char 08 to 0F [non printable]
	0,			0,			0,			0,			0,			0,			0,			0,			// char 10 to 17 [non printable]
	0,			0,			0,			0,			0,			0,			0,			0,			// char 18 to 1F [non printable]
	0b00000000, 0,			0,			0,			0,			0,			0,			0,			// char 20 to 27 space !"#$%&'
	0,			0,			0,			0,			0,			0b00000001, 0,			0,			// char 28 to 2F ()*+,-./
	0b01111110, 0b01000010, 0b00110111, 0b01100111, 0b01001011, 0b01101101, 0b01111101, 0b01000110,	// char 30 to 37 01234567
	0b01111111, 0b01101111, 0,			0,			0,			0,			0,			0,			// char 38 to 3F 89:;<=>?
	0, 			0b01011111,	0b01111001, 0b00110001, 0b01110011, 0,			0b00011101, 0,			// char 40 to 47 @ABCDEFG
	0,			0b01000010, 0b01110010, 0,			0,			0,			0b01010001, 0b01110001, // char 48 to 4F HIJKLMNO
	0b00011111, 0,			0b00010001, 0b01101101, 0b00111001, 0b01110000, 0,			0,			// char 50 to 57 PQRSTUVW
	0,			0b01101011, 0,			0,			0,			0,			0,			0b00100000, // char 58 to 5F XYZ[\]^_
	0, 			0b01011111,	0b01111001, 0b00110001, 0b01110011, 0,			0b00011101, 0,			// char 60 to 67 `abcdefg
	0,			0b01000010, 0b01110010, 0,			0,			0,			0b01010001, 0b01110001, // char 68 to 6F hijklmno
	0b00011111, 0,			0b00010001, 0b01101101, 0b00111001, 0b01110000, 0,			0,			// char 70 to 77 pqrstuvw
	0,			0b01101011, 0,			0,			0,			0,			0,			0, 			// char 78 to 7F xyz{|}~ delete
};

class Segment
{
public:
/**
 * @brief Construct a new Segment object
 * 
 * @param leds 		pointer to array of LEDs starting at this segment's first led
 * @param led_count	number of LED within this segment
 */
	Segment(CRGB *leds, uint8_t led_count)
	{
		m_leds = leds;
		m_led_count = led_count;
	}
	void update_segment(bool on, CRGB colour)
	{
		for (int led_idx = 0; led_idx < m_led_count; led_idx++)
		{
			m_leds[led_idx] = (on) ? colour : CRGB::Black;
		}
	}

private:
	uint8_t m_led_count;
	CRGB *m_leds;
};

class Digit
{
public:
/**
 * @brief Construct a new Digit object
 * 
 * @param leds	pointer to array of LEDs starting at this digit's first led
 */
	Digit(CRGB *leds) : segments {
		Segment(leds + 0 * SEG_LED_CNT_V + 0 * SEG_LED_CNT_H, SEG_LED_CNT_V), // A
		Segment(leds + 1 * SEG_LED_CNT_V + 0 * SEG_LED_CNT_H, SEG_LED_CNT_H), // B
		Segment(leds + 1 * SEG_LED_CNT_V + 1 * SEG_LED_CNT_H, SEG_LED_CNT_V), // C
		Segment(leds + 2 * SEG_LED_CNT_V + 1 * SEG_LED_CNT_H, SEG_LED_CNT_V), // D
		Segment(leds + 3 * SEG_LED_CNT_V + 1 * SEG_LED_CNT_H, SEG_LED_CNT_H), // E
		Segment(leds + 3 * SEG_LED_CNT_V + 2 * SEG_LED_CNT_H, SEG_LED_CNT_V), // F
		Segment(leds + 4 * SEG_LED_CNT_V + 2 * SEG_LED_CNT_H, SEG_LED_CNT_H), // G
	} {	};

	void set(uint8_t digit_to_render, CRGB colour)
	{
		current_value = digit_to_render & 0x7F;	// limit to 128 ascii range
		uint8_t seg_mask = digit_segment_mapping[current_value];
		uint8_t mask = 0b00000001;
		for (int i = 0; i < 7; i++)
		{
			segments[i].update_segment(((mask & seg_mask) != 0), colour);
			mask <<= 1;
		}
	}

private:
	Segment segments[7];
	uint8_t current_value;
};

class Display
{
private:
    Digit digits[6];
	Segment colons[1];
    CRGB leds[LED_COUNT];
public:

	Display() : digits {
		Digit(leds + 0 * DIG_LED_CNT), 
		Digit(leds + 1 * DIG_LED_CNT), 
		Digit(leds + 2 * DIG_LED_CNT), 
		Digit(leds + 3 * DIG_LED_CNT), 
		Digit(leds + 4 * DIG_LED_CNT), 
		Digit(leds + 5 * DIG_LED_CNT)
	} , colons {
		Segment(leds + 6 * DIG_LED_CNT, COL_LED_CNT),
	} {
		FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, LED_COUNT);
		FastLED.setBrightness(255);
	}

	void write_string(const String string, uint8_t length, CRGB colour)
	{
		for (int i = 0; i < length; i++)
		{
			set_digit(5 - i, string.charAt(i), colour);
		}
	}

	void show_time(uint16_t seconds, CRGB colour)
	{
		show_time(seconds, 0, colour, colour);
	}
	void show_time(uint16_t seconds, uint8_t digit_select, CRGB colour, CRGB highlight) {
		uint8_t secs = seconds % 60;
		uint8_t mins = seconds / 60;
		set_digit(0, secs % 10, digit_select == 0 ? highlight : colour);
		set_digit(1, secs / 10, digit_select == 1 ? highlight : colour);
		set_digit(2, mins % 10, digit_select == 2 ? highlight : colour);
		set_digit(3, mins / 10, digit_select == 3 ? highlight : colour);
	}

	void set_digit(uint8_t position, uint8_t value)				{ digits[position].set(value, CRGB::Red);}
	void set_digit(uint8_t position, uint8_t value, CRGB colour)	{ digits[position].set(value, colour);}
	void clear_digit(uint8_t position)								{ digits[position].set(0, CRGB::Black);}

	void push_to_display()
	{
		FastLED.show();
	}

	void clear_display()
	{
		write_string("      ", 6, CRGB::Black);
	}

};

// Display Inferface Handler
extern Display display;
