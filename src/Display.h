#pragma once
#include <FastLED.h>

#define NUM_LEDS 28
#define LED_OFFSET 28

#define DATA_PIN 3

class Display
{

public:
    class Digit
    {
    private:
        const unsigned char digit_segment_mappings[14] = {
            0b1111110, // 0
            0b1000001, // 1
            0b0110111, // 2
            0b1100111, // 3
            0b1001011, // 4
            0b1101101, // 5
            0b1111101, // 6
            0b1000110, // 7
            0b1111111, // 8
            0b1101111, // 9
            0b0111001, // t
            0b1011111, // A
            0b1111001, // b
            0b0111101, // E
        };
        const unsigned char segments[7] = {        //      E           
            0b00000001, // G                             ----
            0b00000010,  // F                            |  B |
            0b00000100,  // E                          C |    | A
            0b00001000,  // D                             ----
            0b00010000,  // C                            | G  |
            0b00100000,  // B                          D |    | F
            0b01000000};  // A                             ----

    public:
        Digit(uint8_t start_index);
        std::array<uint8_t, 28> led_range;
        unsigned char current_value;
        std::array<bool, 7> render_digit(unsigned char digit_to_render);
    };


    Display::Digit digits[6];
    CRGB leds[NUM_LEDS];

    Display();
    void update_display(int position, unsigned char number_to_render);
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