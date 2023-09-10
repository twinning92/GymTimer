#pragma once
#include "Display.h"
class Menu
{
public:
    class Menu_Item
    {
    public:
        enum class item_name
        {
            CLOCK,
            COUNTDOWN,
            COUNTDOWN_ROUND,
            SETTINGS,
        };
        item_name menu_title;
        Menu_Item(item_name name);
        std::array<unsigned char, 6> display_title();
    };

    enum class direction{
        UP,
        DOWN,
    };
    Menu(Display* p_display);
    Display* display;
    Menu::Menu_Item main_menu[4];
    void rotate_menu(Menu::direction direction);

private:
};