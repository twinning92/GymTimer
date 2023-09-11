#include "Display.h"

Display::Digit::Digit(uint8_t start_index)
{
    for (int i = 0; i < 28; i++)
    {
        led_range[i] = start_index + i;
    }
}

std::array<bool, 7> Display::Digit::render_digit(char* digit_to_render)
{
    std::array<bool, 7> segment_states;
    this->current_value = *digit_to_render;

    int int_to_render = std::atoi(digit_to_render);

    for (int i = 0; i < 7; i++)
    {
        segment_states[i] = (segments[i] & digit_segment_mappings[int_to_render]) != 0;
    }
    return segment_states;
}

Display::Display() : digits{Digit(0 * LED_OFFSET),Digit(1 * LED_OFFSET),Digit(2 * LED_OFFSET),Digit(3 * LED_OFFSET),Digit(4 * LED_OFFSET),Digit(5 * LED_OFFSET)}
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed 
    FastLED.setBrightness(255);   
}

void Display::update_display(int position, char number_to_render)
{
    std::array<uint8_t, 28> led_range = digits[position].led_range;
    std::array<bool, 7> segments = digits[position].render_digit(&number_to_render);
    
    for (int segment_index = 0; segment_index < 7; segment_index++)
    {
        bool segment_state = segments[segment_index];

        for (int led_index_in_segment = 0; led_index_in_segment < 3; led_index_in_segment++)
        {
            int absolute_led_index = led_range[segment_index * 3 + led_index_in_segment];
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

void Display::update_segments(int position, int segment, bool value)
{
    std::array<uint8_t, 28> led_range = digits[position].led_range;
    std::array<bool, 7> segments;

    for (int i = 0; i < 7; i++){
        segments[i] = false;
    }

    segments[segment] = value;

    for (int i = 0; i < 7; i++)
    {
        bool segment_state = segments[i];
        for (int j = 0; j < 3; j++)
        {
            int led_index = led_range[i * 3 + j];
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
