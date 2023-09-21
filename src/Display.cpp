#include "Display.h"

// TODO: add using namespace
Display::Digit::Segment::Segment(CRGB *leds, char segment_designator, uint8_t num_leds_per_segment, uint16_t segment_led_offset, uint8_t segment_led_mask)
{
    this->leds = leds;
    this->segment_designator = segment_designator;
    this->num_leds_per_segment = num_leds_per_segment;
    this->start_index = segment_led_offset;
    this->segment_led_mask = segment_led_mask;
}

void Display::Digit::Segment::update_segment(bool on)
{
    for (int led_idx = start_index; led_idx < start_index + num_leds_per_segment; led_idx++)
    {
        if (on)
        {
            leds[led_idx] = CRGB::Amethyst;
        }
        else
        {
            leds[led_idx] = CRGB::Black;
            ;
        }
    }
}

void Display::Digit::Segment::trouble()
{
    for (int i = 0; i < num_leds_per_segment; i++)
    {
        leds[start_index + i] = CRGB::Red;
    }
    FastLED.show();
}

Display::Digit::Digit(CRGB *leds, uint16_t start_index)
{
    segments[0] = Display::Digit::Segment(leds, 'A', 8, start_index, 0b01000000);
    start_index += 8;

    segments[1] = Display::Digit::Segment(leds, 'B', 8, start_index, 0b00100000);
    start_index += 8;

    segments[2] = Display::Digit::Segment(leds, 'C', 8, start_index, 0b00010000);

    start_index += 8;
    segments[3] = Display::Digit::Segment(leds, 'D', 8, start_index, 0b00001000);

    start_index += 8;
    segments[4] = Display::Digit::Segment(leds, 'E', 8, start_index, 0b00000100);

    start_index += 8;
    segments[5] = Display::Digit::Segment(leds, 'F', 8, start_index, 0b00000010);
    start_index += 8;
    segments[6] = Display::Digit::Segment(leds, 'G', 8, start_index, 0b00000001);
}

void Display::Digit::update_digit(uint8_t digit_to_render)
{
    current_value = digit_to_render;
    uint8_t display_mask = digit_segment_mappings[digit_to_render];
    Serial.printf("digit to render: %d display mask: %d \n", digit_to_render, display_mask);
    for (int i = 0; i < 7; i++)
    {
        bool segment_activate = (segments[i].segment_led_mask & display_mask) != 0;
        segments[i].update_segment(segment_activate);
        Serial.printf("render digit %d segment mask %d ", digit_to_render, segments[i].segment_led_mask);
        Serial.printf("segment %c, active %d\n", segments[i].segment_designator, segment_activate);
    }
}

void Display::Digit::trouble()
{
    for (int i = 0; i < 7; i++)
    {
        segments[i].trouble();
        delay(100);
    }
}

Display::Display() : digits{Digit(leds, 0 * LEDS_PER_DIGIT), Digit(leds, 1 * LEDS_PER_DIGIT), Digit(leds, 2 * LEDS_PER_DIGIT), Digit(leds, 3 * LEDS_PER_DIGIT), Digit(leds, 4 * LEDS_PER_DIGIT), Digit(leds, 5 * LEDS_PER_DIGIT)}
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed
    FastLED.setBrightness(10);
}

void Display::update_display(uint8_t position, uint8_t number_to_render)
{
    digits[position].update_digit(number_to_render);
    // Serial.printf("Position: %d Value: %d\n", position, number_to_render);
}

void Display::trouble()
{
    digits[0].trouble();
    digits[1].trouble();
    digits[2].trouble();
    digits[3].trouble();
    digits[4].trouble();
    digits[5].trouble();
    digits[6].trouble();
}

void Display::push_to_display()
{
    FastLED.show();
}
void Display::clear_display()
{
    FastLED.showColor(CRGB::Black);
}
