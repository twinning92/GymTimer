#include "Display.h"

Display::Digit::Digit(uint8_t start_index)
{
    for (int i = 0; i < LEDS_PER_DIGIT; i++)
    {
        led_range[i] = start_index + i;
    }
}

std::array<bool, NUM_SEGMENTS> Display::Digit::render_digit(char* digit_to_render)
{
    std::array<bool, NUM_SEGMENTS> segment_states;
    this->current_value = *digit_to_render;

    // TODO: This is converting char to in, how to do this better with alpha-numerics?
    int int_to_render = std::atoi(digit_to_render);

    for (int i = 0; i < 7; i++)
    {
        segment_states[i] = (segments[i] & digit_segment_mappings[int_to_render]) != 0;
    }
    return segment_states;
}

Display::Display() : digits{Digit(0 * LEDS_PER_DIGIT),Digit(1 * LEDS_PER_DIGIT),Digit(2 * LEDS_PER_DIGIT),Digit(3 * LEDS_PER_DIGIT),Digit(4 * LEDS_PER_DIGIT),Digit(5 * LEDS_PER_DIGIT)}
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed 
    FastLED.setBrightness(255);   
}



void Display::update_display(int position, char number_to_render)
{
    std::array<uint8_t, LEDS_PER_DIGIT> led_range = digits[position].led_range;
    std::array<bool, NUM_SEGMENTS> segments = digits[position].render_digit(&number_to_render);
    
    for (int segment_index = 0; segment_index < NUM_SEGMENTS; segment_index++)
    {
        bool segment_state = segments[segment_index];

        for (int led_index_in_segment = 0; led_index_in_segment < LEDS_PER_SEGMENT; led_index_in_segment++)
        {
            int absolute_led_index = led_range[segment_index * LEDS_PER_SEGMENT + led_index_in_segment];
            if (segment_state)
            {
                leds[absolute_led_index] = CRGB::Red;
            }
            else
            {
                leds[absolute_led_index] = CRGB::Black;
            }
        }
    }
    
    FastLED.show();
}

// Trouble shooting function to isolate a specific segement.
void Display::update_segments(int position, int segment, bool value)
{
    std::array<uint8_t, LEDS_PER_DIGIT> led_range = digits[position].led_range;
    std::array<bool, NUM_SEGMENTS> segments;

    for (int i = 0; i < NUM_SEGMENTS; i++){
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


void Display::clear_display()
{
    for (int led_index = 0; led_index < NUM_LEDS; led_index++)
    {
        leds[led_index] = CRGB::Black;
    }
    FastLED.show();
}
