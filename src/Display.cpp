#include "Display.h"

// TODO: add using namespace
Display::Digit::Segment::Segment(CRGB &leds, char segment_designator, uint8_t num_leds_per_segment, uint16_t segment_led_offset)
{
    leds = leds;
    segment_designator = segment_designator;
    num_leds_per_segment = num_leds_per_segment;
    segment_led_offset = segment_led_offset;
}

void Display::Digit::Segment::update_segment(bool on, CRGB colour)
{
    for (int led_idx = segment_led_offset; led_idx < segment_led_offset + num_leds_per_segment; led_idx++)
    {
        if (on)
        {
            FastLED
        }
    }
}

Display::Digit::Digit(CRGB &leds, uint16_t start_index)
{
    segments[0] = Display::Digit::Segment(*leds, 'A', 8, start_index);
    start_index += 8;
    segments[1] = new Display::Digit::Segment(leds, 'B', 9, start_index);
    start_index += 9;
    segments[2] = Display::Digit::Segment(leds, 'C', 8, start_index);
    start_index += 8;
    segments[3] = Display::Digit::Segment(leds, 'D', 8, start_index);
    start_index += 8;
    segments[4] = Display::Digit::Segment(leds, 'E', 9, start_index);
    start_index += 9;
    segments[5] = Display::Digit::Segment(leds, 'F', 8, start_index);
    start_index += 8;
    segments[6] = Display::Digit::Segment(leds, 'G', 9, start_index);
}

Display::Display() : digits{Digit(*leds, 0 * LEDS_PER_DIGIT), Digit(*leds, 1 * LEDS_PER_DIGIT), Digit(*leds, 2 * LEDS_PER_DIGIT), Digit(*leds, 3 * LEDS_PER_DIGIT), Digit(*leds, 4 * LEDS_PER_DIGIT), Digit(*leds, 5 * LEDS_PER_DIGIT)}
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed

    FastLED.setBrightness(50);
}

void Display::Digit::update_digit(uint8_t digit_to_render)
{
    current_value = digit_to_render;
    uint8_t segment_mask = digit_segment_mappings[digit_to_render];

    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        uint8_t mask = 1 << i;
        segments[i].update_segment((mask & segment_mask) != 0);
    }
}

void Display::update_display(uint8_t position, uint8_t number_to_render)
{
    digits[position].update_digit(number_to_render);
}

void Display::push_to_display()
{
    FastLED.show();
}
