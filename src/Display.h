#pragma once
#include <FastLED.h>

#define NUM_LEDS 354
#define NUM_SEGMENTS 7
#define LEDS_PER_DIGIT 59 / 6

#define DATA_PIN 3

class Display
{

public:
    class Digit
    {
    public:
        Digit(CRGB& leds, uint16_t start_index);
        void update_digit(uint8_t digit_to_render);
        void show_digit(bool on);

    private:
        class Segment
        {
        public:
            Segment(CRGB& leds, char designator, uint8_t size, uint16_t segment_led_offset);
            Segment() = default;
            void update_segment(bool on, CRGB colour);

        private:
            char segment_designator;
            uint8_t num_leds_per_segment;
            uint16_t segment_led_offset; // index of the zeroth segment led in the entire led strip.
            CRGB& leds;
        };

        Display::Digit::Segment segments[7];
        uint8_t current_value;
        uint16_t digit_led_offset;


        // TODO: talk to chat gpt about how to make this a global table.
        const uint8_t digit_segment_mappings[14] = {
            0b01111110, // 0
            0b01000010, // 1
            0b00110111, // 2
            0b01100111, // 3
            0b01001011, // 4
            0b01101101, // 5
            0b01111101, // 6
            0b01000110, // 7
            0b01111111, // 8
            0b01101111, // 9
            0b00111001, // t
            0b01011111, // A
            0b01111001, // b
            0b00111101, // E
        };
    };

public:
    Display();
    void push_to_display();
    void update_display(uint8_t position, uint8_t number_to_render);

private:
    Display::Digit digits[6];
    CRGB leds[NUM_LEDS];
};

/*

A	 B	  C	   D    E    F    G
1111 1111 1111 1111 1111 1111 0000	// 0

1111 0000 0000 0000 0000 0000 1111  // 1

0000 1111 1111 0000 1111 1111 1111  // 2

1111 1111 0000 0000 1111 1111 1111  // 3

1111 0000 0000 1111 0000 1111 1111  // 4

1111 1111 0000 1111 1111 0000 1111  // 5

1111 1111 1111 1111 1111 0000 1111  // 6

1111 0000 0000 0000 1111 1111 0000  // 7

1111 1111 1111 1111 1111 1111 1111  // 8

1111 1111 0000 1111 1111 1111 1111  // 9

0000 1111 1111 1111 0000 0000 1111  // t

1111 0000 1111 1111 1111 1111 1111  // A

1111 1111 1111 1111 0000 0000 1111  // b

0000 1111 1111 1111 1111 0000 1111  // E
*/