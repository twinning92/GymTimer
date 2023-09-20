#include "Display.h"

Display::Digit::Segment::Segment(char designator, uint8_t size, uint8_t segment_mask)
{
    segment_mask = segment_mask;
    segment_designator = designator;
    num_leds_per_segment = size;
}

Display::Digit::Digit(uint16_t start_index) : segment{Segment('A', 8, 0b01000000), Segment('B', 9, 0b00100000), 
                                                      Segment('C', 8, 0b00010000), Segment('D', 8, 0b00001000),
                                                      Segment('E', 9, 0b00000100), Segment('F', 8, 0b00000010), 
                                                      Segment('G', 9, 0b00000001)}
{
    for (int i = 0; i < LEDS_PER_DIGIT; i++)
    {
        led_range[i] = start_index + i;
    }
}

Display::Display() : digits{Digit(0 * LEDS_PER_DIGIT), Digit(1 * LEDS_PER_DIGIT), Digit(2 * LEDS_PER_DIGIT), Digit(3 * LEDS_PER_DIGIT), Digit(4 * LEDS_PER_DIGIT), Digit(5 * LEDS_PER_DIGIT)}
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed

    FastLED.setBrightness(50);
}

std::array<bool, NUM_SEGMENTS> Display::Digit::render_digit(uint8_t digit_to_render)
{
    std::array<bool, NUM_SEGMENTS> segment_states;
    this->current_value = digit_to_render;

    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        segment_states[i] = (segment[i].segment_mask & digit_segment_mappings[digit_to_render]) != 0;
        
    }
    return segment_states;
}

void Display::update_display(uint8_t position, uint8_t number_to_render)
{
    std::array<uint16_t, LEDS_PER_DIGIT> led_range = digits[position].led_range;
    std::array<bool, NUM_SEGMENTS> segments = digits[position].render_digit(number_to_render);

    for (int segment_index = 0; segment_index < NUM_SEGMENTS; segment_index++)
    {
        bool segment_state = segments[segment_index];

        for (int led_index_in_segment = 0; led_index_in_segment < digits[position].segment[segment_index].num_leds_per_segment; led_index_in_segment++)
        {
            int absolute_led_index = led_range[segment_index * LEDS_PER_SEGMENT + led_index_in_segment];
            if (segment_state)
            {
                leds[absolute_led_index] = CRGB::Red;
                Serial.print(" On: absolute index: ");
                Serial.println(absolute_led_index);
            }
            else
            {
                leds[absolute_led_index] = CRGB::Black;
                Serial.print(" Off: absolute index: ");
                Serial.println(absolute_led_index);
            }
        }
    }
}

// Trouble shooting function to isolate a specific segement.
void Display::update_segments(uint8_t position, uint8_t segment, bool value)
{
    std::array<uint16_t, LEDS_PER_DIGIT> led_range = digits[position].led_range;
    std::array<bool, NUM_SEGMENTS> segments;

    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        segments[i] = false;
    }

    segments[segment] = value;

    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        bool segment_state = segments[i];
        for (int j = 0; j < LEDS_PER_SEGMENT; j++)
        {
            int led_index = led_range[i * LEDS_PER_SEGMENT + j];
            if (segment_state)
            {
                leds[led_index] = CRGB::Red;
            }
            else
            {
                leds[led_index] = CRGB::Black;
            }
        }
    }
    FastLED.show();
}

void Display::push_to_display()
{
    FastLED.show();
}

void Display::clear_display()
{
    for (int led_index = 0; led_index < NUM_LEDS; led_index++)
    {
        leds[led_index] = CRGB::Black;
    }
    FastLED.show();
}
