#include "Menu.h"

Menu::Menu_Item::Menu_Item(Menu::Menu_Item::item_name type)
{
    this->menu_title = type;

    std::array<unsigned char, 6> title_array;
    switch (Menu::Menu_Item::menu_title)
    {
    case Menu::Menu_Item::item_name::CLOCK:
        title_array[5] = 'c';
        title_array[4] = 'l';
        title_array[3] = 'o';
        title_array[2] = 'c';
        title_array[1] = 'k';
        title_array[0] = ' ';
        break;
    case Menu::Menu_Item::item_name::COUNTDOWN:
        title_array[5] = 'c';
        title_array[4] = 'n';
        title_array[3] = 't';
        title_array[2] = ' ';
        title_array[1] = 'd';
        title_array[0] = 'n';
        break;
    case Menu::Menu_Item::item_name::COUNTDOWN_ROUND:
        title_array[5] = 'c';
        title_array[4] = 'n';
        title_array[3] = 't';
        title_array[2] = 'r';
        title_array[1] = 'n';
        title_array[0] = 'd';
        break;
    case Menu::Menu_Item::item_name::SETTINGS:
        title_array[5] = 's';
        title_array[4] = 'e';
        title_array[3] = 't';
        title_array[2] = 't';
        title_array[1] = 'i';
        title_array[0] = 'n';
        break;
    }
}
std::array<unsigned char, 6> Menu::Menu_Item::display_title()
{
    
}


Menu::Menu(Display *p_display) : main_menu{Menu_Item(Menu_Item::item_name::CLOCK),
                                           Menu_Item(Menu_Item::item_name::COUNTDOWN),
                                           Menu_Item(Menu_Item::item_name::COUNTDOWN_ROUND),
                                           Menu_Item(Menu_Item::item_name::SETTINGS)}
{
    display = p_display;
}

void Menu::rotate_menu(Menu::direction direction)
{
    switch (direction)
    {
    case Menu::direction::UP:
        display->update_display() break;
    case Menu::direction::DOWN:
        break;
    }
}