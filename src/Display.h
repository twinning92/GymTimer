#pragma once
#include <FastLED.h>

#define LEDS_PER_DIGIT ((4 * 8) + (3 * 9))
#define NUM_LEDS ((LEDS_PER_DIGIT * 6) + 8)
#define NUM_SEGMENTS 7

#define DATA_PIN 15

class Segment
{
public:
    Segment(CRGB *leds, char designator, uint8_t size, uint16_t start_index, uint8_t segment_led_mask);
    void update_segment(bool on, CRGB colour);
    uint8_t segment_led_mask;

    // TODO: These are only public for serial output in update_segements()
    char segment_designator;
    uint16_t start_index;

private:
    Segment() = default;
    CRGB *leds;
    uint8_t num_leds_per_segment;
};

class Digit
{
public:
    Digit(CRGB *leds, uint16_t start_index);
    void update_digit(uint8_t digit_to_render, CRGB colour);
    void show_digit(bool on);

private:
    Segment segments[7];
    uint8_t current_value;
    uint16_t digit_led_offset;

    // TODO: talk to chat gpt about how to make this a global table.
    const uint8_t digit_segment_mappings[50] = {
        0b01111110, // 0 0
        0b01000010, // 1 1
        0b00110111, // 2 2
        0b01100111, // 3 3
        0b01001011, // 4 4
        0b01101101, // 5 5
        0b01111101, // 6 6
        0b01000110, // 7 7
        0b01111111, // 8 8
        0b01101111, // 9 9
        0b01011111, // A 10
        0b01111001, // b 11
        0b00110001, // c 12
        0b01110011, // d 13
        0b00111101, // E 14
        0b01010001, // n 15
        0b01110001, // o 16
        0b00010001, // r 17
        0b00111001, // t 18
        0b01000010, // l,i 19
        0b00011101, // F 20
        0b01110000, // u 21
        0b00011111, // P 22
        0b01110010, // J 23
        0b01101101, // S 24
        0b00000000, // " " 25
        0b00100000, // "_" 26
        0b00000001, // "-" 27
        0b01101011, // "y" 28
    };
};

class Display
{
public:
    Display();
    void push_to_display();
    void write_string(const String, uint8_t length, CRGB colour);
    void convert_to_display(const unsigned int total_seconds, CRGB colour);
    void update_display(uint8_t position, uint8_t number_to_render);
    void update_display(uint8_t position, uint8_t number_to_render, CRGB colour);
    void clear_digit(uint8_t position);
    void clear_display();

private:
    Digit digits[6];
    CRGB leds[NUM_LEDS];
};