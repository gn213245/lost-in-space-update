#ifndef POWER_UP
#define POWER_UP

#include <string>
using std::string;
#include <vector>
using std::vector;
#include "splashkit.h"

#define SCREEN_BORDER 100

enum power_up_kind
{
    FUEL,
    BATTERY,
    COIN,
    SHIELD,
    CASH,
    TIME
};

struct power_up_data
{
    power_up_kind kind;
    sprite power_up_sprite;
    bool power_up_visit;
    int duration;
};

bitmap power_up_bitmap(power_up_kind kind);
power_up_data new_power_up(double x, double y);
void draw_power_up(const power_up_data &power_up_to_draw);
void update_power_up(power_up_data &power_up_to_update);
#endif