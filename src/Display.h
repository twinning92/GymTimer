#pragma once
#include <FastLED.h>

#define NUM_LEDS 336
#define NUM_SEGMENTS 7
#define LEDS_PER_DIGIT NUM_LEDS / 6
#define LEDS_PER_SEGMENT LEDS_PER_DIGIT / NUM_SEGMENTS

#define DATA_PIN 3

class Display
{

public:
    class Digit
    {
    private:
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
        const uint8_t segments[7] = {              
            0b01000000,  // A                      
            0b00100000,  // B                      
            0b00010000,  // C                      
            0b00001000,  // D                      
            0b00000100,  // E                      
            0b00000010,  // F                      
            0b00000001,  // G
            };
                           
                                                   

    public:
        Digit(uint16_t start_index);
        std::array<uint16_t, LEDS_PER_DIGIT> led_range;
        uint8_t current_value;
        std::array<bool, NUM_SEGMENTS> render_digit(uint8_t digit_to_render);
    };


    Display::Digit digits[6];
    CRGB leds[NUM_LEDS];

    Display();
    void push_to_display();
    void update_display(uint8_t position, uint8_t number_to_render);
    void update_segments(uint8_t position, uint8_t segment, bool value);
    void clear_display();
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