#include "Display.h"

Display::Digit::Digit(uint8_t start_index)
{
    for (int i = 0; i < 28; i++)
    {
        led_range[i] = start_index + i;
    }
}

std::array<bool, 7> Display::Digit::render_digit(unsigned char digit_to_render)
{
    std::array<bool, 7> segment_states;
    this->current_value = digit_to_render;

    for (int i = 0; i < 7; i++)
    {
        segment_states[i] = (segments[i] & digit_segment_mappings[digit_to_render]);
    }
    return segment_states;
}

Display::Display() : digits{Digit(0 * LED_OFFSET),Digit(1 * LED_OFFSET),Digit(2 * LED_OFFSET),Digit(3 * LED_OFFSET),Digit(4 * LED_OFFSET),Digit(5 * LED_OFFSET)}
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed    
}

void Display::update_display(int position, unsigned char number_to_render)
{
    std::array<uint8_t, 28> led_range = digits[position].led_range;
    std::array<bool, 7> segments = digits[position].render_digit(number_to_render);
    
    this->digits[position].current_value = number_to_render;

    for (int i = 0; i < 7; i++)
    {
        bool segment_state = segments[i];
        for (int j = 0; j < 4; j++)
        {
            int led_index = led_range[i * 4 + j];
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
