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
    for (int led_idx = this->start_index; led_idx < this->start_index + this->num_leds_per_segment; led_idx++)
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
                                                     Segment(leds, 'B', 9, start_index + 1 * 8, 0b00100000),
                                                     Segment(leds, 'C', 8, start_index + 1 * 8 + 1 * 9, 0b00010000),
                                                     Segment(leds, 'D', 8, start_index + 2 * 8 + 1 * 9, 0b00001000),
                                                     Segment(leds, 'E', 9, start_index + 3 * 8 + 1 * 9, 0b00000100),
                                                     Segment(leds, 'F', 8, start_index + 3 * 8 + 2 * 9, 0b00000010),
                                                     Segment(leds, 'G', 9, start_index + 4 * 8 + 2 * 9, 0b00000001)}
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
        // //Serial.printf("Segment letter: %c, start index: %d\n", segments[i].segment_designator, segments[i].start_index);
    }
}

Display::Display() : digits{Digit(leds, 0 * LEDS_PER_DIGIT), Digit(leds, 1 * LEDS_PER_DIGIT), Digit(leds, 2 * LEDS_PER_DIGIT), Digit(leds, 3 * LEDS_PER_DIGIT), Digit(leds, 4 * LEDS_PER_DIGIT), Digit(leds, 5 * LEDS_PER_DIGIT)}
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(255);
}

void Display::write_string(const String string, uint8_t length, CRGB colour)
{
    //Serial.printf("Printing: \n");
    for (int i = 0; i < length; i++)
    {
        switch (string.charAt(i))
        {
            // Basically need to reverse the display, string comes in left-right but digit mappings are right-left
        case 'A':
        case 'a':
            update_display(5 - i, 10, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));

            break;
        case 'b':
            update_display(5 - i, 11, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));

            break;
        case 'c':
            update_display(5 - i, 12, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 'd':
            update_display(5 - i, 13, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 'E':
        case 'e':
            update_display(5 - i, 14, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 'n':
            update_display(5 - i, 15, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 'o':
            update_display(5 - i, 16, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 'r':
            update_display(5 - i, 17, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 't':
            update_display(5 - i, 18, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 'l':
        case 'i':
            update_display(5 - i, 19, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 'F':
        case 'f':
            update_display(5 - i, 20, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 'u':
        case 'U':
            update_display(5 - i, 21, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 'p':
        case 'P':
            update_display(5 - i, 22, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 'J':
        case 'j':
            update_display(5 - i, 23, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 'S':
        case 's':
            update_display(5 - i, 24, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case ' ':
            update_display(5 - i, 25, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case '_':
            update_display(5 - i, 26, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case '-':
            update_display(5 - i, 27, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;
        case 'y':
        case 'Y':
            update_display(5 - i, 28, colour);
            //Serial.printf("%d: %c\n", i, string.charAt(i));
            break;

        default:
            clear_digit(5 - i);
            //Serial.printf("default: clearing digit. unable to print '%c'\n", i, string.charAt(i));
            break;
        }
    }
    //Serial.printf("length: %d\n", length);
}

void Display::convert_to_display(const unsigned int total_seconds, CRGB colour)
{
    uint8_t low_seconds = total_seconds % 10;
    uint8_t high_seconds = total_seconds / 10 % 6;

    uint8_t total_minutes = (total_seconds / 60) % 60;
    uint8_t low_minutes = total_minutes % 10;
    uint8_t high_minutes = total_minutes / 10;

    update_display(0, low_seconds, colour);
    update_display(1, high_seconds, colour);
    update_display(2, low_minutes, colour);
    update_display(3, high_minutes, colour);
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
    write_string("      ", 6, CRGB::Black);
}
