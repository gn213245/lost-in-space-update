#include "splashkit.h"
#include "lost_in_space.h"
#include "player.h"
#include "planet.h"
#include "power_up.h"

#include <vector>
using std::vector;
using namespace std;
void draw_hud(const vector<planet_data> &planets, const player_data &player)
{
    // load_bitmap("green_bar_lines", "green_bar_lines.png");
    draw_text("BATTERY:", COLOR_WHITE, 10, 50, option_to_screen());
    draw_bitmap("empty_bar_transparent", 70, 30, option_to_screen());
    draw_bitmap("green_bar_lines", 70, 30, option_part_bmp(0, 0, bitmap_width("green_bar_lines") * player.battery_pct, bitmap_height("green_bar_lines"), option_to_screen()));
    draw_bitmap("empty_bar_transparent", 450, 30, option_to_screen());
    draw_text("SHIELD:", COLOR_WHITE, 400, 50, option_to_screen());
    draw_bitmap("purple_bar", 450, 30, option_part_bmp(0, 0, bitmap_width("purple_bar") * player.shield_pct, bitmap_height("purple_bar"), option_to_screen()));
    draw_text("FUEL:", COLOR_WHITE, 10, 95, option_to_screen());
    draw_bitmap("bar_white_fill", 45, 75, option_to_screen());
    draw_bitmap("orange_bar_lines", 45, 75, option_part_bmp(0, 0, bitmap_width("orange_bar_lines") * player.fuel_pct, bitmap_height("orange_bar_lines"), option_to_screen()));
    game_data game;  
    int planet_index = closest_planet_index(player, planets);
    //x and y are the position coordinates of player sprite
    float x = sprite_x(player.player_sprite);
    float y = sprite_y(player.player_sprite);
    //To store the center points of both planet and player
    point_2d planet_center, player_center;
    planet_center = center_point(planets[planet_index].planet_sprite);
    player_center = center_point(player.player_sprite);
    //Calculate distance between center of planet and center of player
    //float point_point_distance(const point_2d &pt1, const point_2d &pt2)
    float distance;
    distance = point_point_distance(planet_center, player_center);
    //container for the whole draw thing
    fill_rectangle(COLOR_WHITE, 20, 135, 300, 60, option_to_screen());
    //draw some text
    draw_text("SCORE: " + to_string(player.score), COLOR_BLACK, 20, 140, option_to_screen());
    draw_text("COIN: " + to_string(player.coin), COLOR_BLACK, 20, 150, option_to_screen());
    draw_text("LOCATION: " + to_string(x) + ", " + to_string(y), COLOR_BLACK, 20, 160, option_to_screen());
    
    draw_text("DISTANCE TO CLOSEST PLANET: " + to_string(distance), COLOR_BLACK, 20, 170, option_to_screen());
    
    draw_text("Timer is: " + to_string(player.timer), COLOR_BLACK, 20, 180, option_to_screen());
}

//Generates planets at random positions and player at the center
void setup_level(game_data &game, int width, int height, int num_planets)
{
    reset_timer("TIMER");
    //Position in the centre of the initial screen using height and width

    //creating indicated(num_planets) number of random planets
    for (int i = 0; i < num_planets; i++)
    {
        //new_planet(double x, double y), generating random values of x and y coordinates
        game.planets.push_back(new_planet(rnd(3000), rnd(3000)));
    }
}



game_data new_game()
{
    game_data game;
    //storing new_player() inside game_data
    game.player = new_player();
    game.game_timer = create_timer("TIMER");
    //reset_timer("TIMER");
    start_timer(game.game_timer);
    setup_level(game, 3000, 3000, 10);

    return game;
}

void draw_game(game_data &game)
{
    // int planet_index = closest_planet_index(game.player, game.planets);
    //draw everything
    clear_screen(COLOR_BLACK);

    draw_hud(game.planets, game.player);

    // Draw planet
    for (int i = 0; i < game.planets.size(); i++)
    {
        draw_planet(game.planets[i]);
    }
    // as well as the player who can move
    draw_player(game.player);
    
    for (int i = 0; i < game.power_ups.size(); i++)
    {
        draw_power_up(game.power_ups[i]);
    }
    refresh_screen(60);
}

void add_power_up(game_data &game)
{
    game.power_ups.push_back(new_power_up(rnd(-800, 800), rnd(-800, 800)));
}

void remove_power_up(game_data &game, int index)
{
    if (index > 0 and index < game.power_ups.size())
    {
        int last_idx;
        last_idx = game.power_ups.size() - 1;
        game.power_ups[index] = game.power_ups[last_idx];
        game.power_ups.pop_back();
    }
    
}
void apply_power_up(player_data &player, power_up_data &power_up)
{   
    game_data game;
    
    switch (power_up.kind)
    {
        case FUEL:
            if (player.fuel_pct < 1)
            {
                player.fuel_pct = player.fuel_pct + 0.1;
            }
        case BATTERY:
            if (player.battery_pct < 1)
            {
                player.battery_pct += 0.1;
            }
            break;
        case CASH:
            player.score = player.score + 1;
            player.coin = player.coin + 5;
            break;
        case COIN:
            player.coin = player.coin + 1;
            break;
        case TIME:
            player.score = player.score + 1;
            player.timer += 500;
            break;
        case SHIELD:
            player.score = player.score + 1;
            if (player.shield_pct < 1)
            {
                player.shield_pct += 0.25;
            }
            
            break;
    }
    power_up.duration = 0;

}

void end_game(const game_data& game)
{
    clear_screen(COLOR_BLACK);
    draw_text("GAME OVER", COLOR_WHITE, screen_width() / 2 - 50, screen_height() / 2);
    draw_text("SCORE: " + to_string(game.player.score), COLOR_WHITE, screen_width() / 2 -50, screen_height() / 2 +30);
    refresh_screen(60);
    delay(3000);
}


void update_game(game_data &game)
{
    update_player(game.player);
    for (int i = 0; i < game.planets.size(); i++)
    {
        update_planet(game.planets[i]);
    }
    game.player.fuel_pct = game.player.fuel_pct - 0.000278;
    game.player.battery_pct -= 0.0005;
    if (rnd() < 0.05)
    {
        add_power_up(game);
    }

    if (game.player.timer > 0)
    {
        game.player.timer -=1;

    }
    
    

    for (int i = 0; i < game.power_ups.size(); i++)
    {
        update_power_up(game.power_ups[i]);
        if (game.power_ups[i].duration > 0)
        {
            game.power_ups[i].duration--;
            if (game.player.timer < 0)
            {
                game.player.timer = 0;
            }
            
        }
        else
        {
            remove_power_up(game, i);
        }
        
    }
    if (game.player.timer == 0)
    {
        end_game(game);
    }
    if (game.player.fuel_pct < 0)
    {
        game.player.fuel_pct = 0;
    }
    if (game.player.battery_pct < 0)
    {
        game.player.battery_pct = 0;
    }
    
    check_collisions(game);

}

//returns index of closest planet to the player
int closest_planet_index(const player_data &player, const vector<planet_data> &planets)
{
    //If All the planets have been visited
    int index = -1;
    //The distance from the first planet as a reference
    point_2d player_center = center_point(player.player_sprite);
    point_2d test_center = center_point(planets[0].planet_sprite);
    vector_2d player_to_test = vector_point_to_point(player_center, test_center);

    double test_distance = vector_magnitude(player_to_test);
    double distance = test_distance;

    for (int i = 1; i < planets.size(); i++)
    {
        if (planets[i].planet_visit == false)
        {
            test_center = center_point(planets[i].planet_sprite);
            player_to_test = vector_point_to_point(player_center, test_center);
            test_distance = vector_magnitude(player_to_test);

            if (test_distance < distance || index == -1)
            {
                distance = test_distance;
                index = i;
            }
        }
    }
    return index;
}

void check_collisions(game_data &game)
{
    for (int i = 0; i < game.planets.size(); i++)
    {
        if (sprite_collision(game.player.player_sprite, game.planets[i].planet_sprite))
        {
            
            if (game.planets[i].planet_visit == false)
            {
                game.player.shield_pct -= 0.25;

                // If shield_pct drops below 0, set it to 0
                if (game.player.shield_pct < 0)
                {
                    game.player.shield_pct = 0;
                }
                game.planets[i].planet_visit = true;
                // game.planets[i].planet_visit = true;
                if (game.player.shield_pct == 0)
                {
                    game.player.score--;
                    if (game.player.score < 0)
                    {
                        game.player.score = 0;
                    }
                    game.player.shield_pct = 0;
                }
                load_sound_effect("boom", "electronic-impact-hard-10018.mp3");
                play_sound_effect("boom");
                reset_timer("TIMER");
            }
        }
    }
    for (int i = 0; i < game.power_ups.size(); i++)
    {
        if (sprite_collision(game.player.player_sprite, game.power_ups[i].power_up_sprite))
        {
            apply_power_up(game.player, game.power_ups[i]);
            remove_power_up(game, i);
            load_sound_effect("yes", "yes.wav");
            play_sound_effect("yes");
        }
    }
    
    if (closest_planet_index(game.player, game.planets) == -1)
    {
        setup_level(game, 3000, 3000, 5);
    }
}
