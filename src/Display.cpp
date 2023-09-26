#include "Display.h"

Segment::Segment(CRGB *leds, char segment_designator, uint8_t num_leds_per_segment, uint16_t segment_led_offset, uint8_t segment_led_mask)
{
    this->leds = leds;
    this->segment_designator = segment_designator;
    this->num_leds_per_segment = num_leds_per_segment;
    this->start_index = segment_led_offset;
    this->segment_led_mask = segment_led_mask;
}

void Segment::update_segment(bool on, CRGB colour)
{
    for (int led_idx = start_index; led_idx < start_index + num_leds_per_segment; led_idx++)
    {
        if (on)
        {
            leds[led_idx] = colour;
        }
        else
        {
            leds[led_idx] = CRGB::Black;
        }
    }
}

Digit::Digit(CRGB *leds, uint16_t start_index) : segments{
                                                     Segment(leds, 'A', 8, start_index + 0 * 8, 0b01000000),
                                                     Segment(leds, 'B', 8, start_index + 1 * 8, 0b00100000),
                                                     Segment(leds, 'C', 8, start_index + 2 * 8, 0b00010000),
                                                     Segment(leds, 'D', 8, start_index + 3 * 8, 0b00001000),
                                                     Segment(leds, 'E', 8, start_index + 4 * 8, 0b00000100),
                                                     Segment(leds, 'F', 8, start_index + 5 * 8, 0b00000010),
                                                     Segment(leds, 'G', 8, start_index + 6 * 8, 0b00000001)}
{
}

void Digit::update_digit(uint8_t digit_to_render, CRGB colour)
{
    current_value = digit_to_render;
    uint8_t display_mask = digit_segment_mappings[digit_to_render];
    for (int i = 0; i < 7; i++)
    {
        bool segment_activate = (segments[i].segment_led_mask & display_mask) != 0;
        segments[i].update_segment(segment_activate, colour);
    }
}

Display::Display() : digits{Digit(leds, 0 * LEDS_PER_DIGIT), Digit(leds, 1 * LEDS_PER_DIGIT), Digit(leds, 2 * LEDS_PER_DIGIT), Digit(leds, 3 * LEDS_PER_DIGIT), Digit(leds, 4 * LEDS_PER_DIGIT), Digit(leds, 5 * LEDS_PER_DIGIT)}
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(10);
}

void Display::write_string(const std::string string, uint8_t length, CRGB colour)
{
    clear_display();

    for (int i = 0; i < length; i++)
    {
        switch (string[i])
        {
            // Basically need to reverse the display, string comes in left-right but digit mappings are right-left
        case 'A':
            update_display(length - i, 10, colour);
            break;
        case 'b':
            update_display(length - i, 11, colour);
            break;
        case 'c':
            update_display(length - i, 12, colour);
            break;
        case 'd':
            update_display(length - i, 13, colour);
            break;
        case 'E':
            update_display(length - i, 14, colour);
            break;
        case 'n':
            update_display(length - i, 15, colour);
            break;
        case 'o':
            update_display(length - i, 16, colour);
            break;
        case 'r':
            update_display(length - i, 17, colour);
            break;
        case 't':
            update_display(length - i, 18, colour);
            break;
        case 'l':
        case 'i':
            update_display(length - i, 19, colour);
            break;
        case 'F':
        case 'f':
            update_display(length - i, 20, colour);
            break;
        case 'u':
        case 'U':
            update_display(length - i, 21, colour);
            break;
        case 'p':
        case 'P':
            update_display(length - i, 22, colour);
            break;
        case 'J':
            update_display(length - i, 23, colour);
            break;
        case 'S':
            update_display(length - i, 24, colour);
            break;
        default:
            clear_digit(length - i);
            break;
        }
    }
}

void Display::convert_to_display(const unsigned int total_seconds)
{
    uint8_t low_seconds = total_seconds % 10;
    uint8_t high_seconds = total_seconds / 10 % 6;

    uint8_t total_minutes = (total_seconds / 60) % 60;
    uint8_t low_minutes = total_minutes % 10;
    uint8_t high_minutes = total_minutes / 10;

    update_display(0, low_seconds);
    update_display(1, high_seconds);
    update_display(2, low_minutes);
    update_display(3, high_minutes);
}

void Display::update_display(uint8_t position, uint8_t number_to_render)
{
    digits[position].update_digit(number_to_render, CRGB::Red);
}

void Display::update_display(uint8_t position, uint8_t number_to_render, CRGB colour)
{
    digits[position].update_digit(number_to_render, colour);
}

void Display::clear_digit(uint8_t position)
{
    digits[position].update_digit(0, CRGB::Black);
}

void Display::push_to_display()
{
    FastLED.show();
}
void Display::clear_display()
{
    FastLED.showColor(CRGB::Black);
}
