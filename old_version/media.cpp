/****************************
 BUBBLE DRIVE
 2011/2012 STOMPY BLONDIE GAMES
 ****************************
 Media object source file
 ****************************/


#include <string>
using namespace std;

#include "main.h"


Media::Media()
{

    // Load all the fonts!
    fonts.insert(pair<string,Font*>("basic", new Font("fnt/aurulent.ttf", 13)));
    fonts.insert(pair<string,Font*>("fixed", new Font("fnt/bitmap1.ttf", 10)));
    fonts.insert(pair<string,Font*>("fixed_small", new Font("fnt/bitmap1.ttf", 8)));

    // Visuals
    gfx.insert(pair<string, Image*>("gui_stompyblondie_logo", new Image("gfx/gui/stompyblondie_logo.png")));
    gfx.insert(pair<string, Image*>("gui_stompyblondie_logo_text", new Image("gfx/gui/stompyblondie_logo_text.png")));

    gfx.insert(pair<string, Image*>("gui_cursor", new Image("gfx/gui/cursor.png")));
    gfx.insert(pair<string, Image*>("gui_crosshair", new Image("gfx/gui/crosshair.png")));

    gfx.insert(pair<string, Image*>("background_space_dust_1", new Image("gfx/background/space_dust_1.png")));
    gfx.insert(pair<string, Image*>("background_space_dust_2", new Image("gfx/background/space_dust_2.png")));
    gfx.insert(pair<string, Image*>("background_space_dust_3", new Image("gfx/background/space_dust_3.png")));
    gfx.insert(pair<string, Image*>("background_space_dust_4", new Image("gfx/background/space_dust_4.png")));
    gfx.insert(pair<string, Image*>("background_stars", new Image("gfx/background/stars.png", True)));
    gfx.insert(pair<string, Image*>("background_nebula1", new Image("gfx/background/nebula1.png", False, 8)));
    gfx.insert(pair<string, Image*>("background_nebula2", new Image("gfx/background/nebula2.png", False, 8)));
    gfx.insert(pair<string, Image*>("background_nebula3", new Image("gfx/background/nebula3.png", False, 8)));

    gfx.insert(pair<string, Image*>("background_objects_planet1", new Image("gfx/background/objects/planet1.png")));
    gfx.insert(pair<string, Image*>("background_objects_planet2", new Image("gfx/background/objects/planet3.png")));
    gfx.insert(pair<string, Image*>("background_objects_planet_broken1", new Image("gfx/background/objects/planet_broken1.png")));
    gfx.insert(pair<string, Image*>("background_objects_star1", new Image("gfx/background/objects/star1.png")));
    gfx.insert(pair<string, Image*>("background_objects_star1_over", new Image("gfx/background/objects/star1_over.png")));

    gfx.insert(pair<string, Image*>("ships_ship2", new Image("gfx/ships/ship2.png")));

    gfx.insert(pair<string, Image*>("weapons_laser1", new Image("gfx/weapons/laser1.png", False, 2)));

    gfx.insert(pair<string, Image*>("world_objects_asteroid1", new Image("gfx/world_objects/asteroid1.png")));

    gfx.insert(pair<string, Image*>("fx_engine_glow1", new Image("gfx/fx/engine_glow1.png")));

/*
    gfx.insert(pair<string, Image*>("gui_button_generic_background", new Image("gfx/gui/button_generic_background.png", True, 4)));
    gfx.insert(pair<string, Image*>("gui_button_spinner_down", new Image("gfx/gui/button_spinner_down.png", False, 3)));
    gfx.insert(pair<string, Image*>("gui_button_spinner_up", new Image("gfx/gui/button_spinner_up.png", False, 3)));
    gfx.insert(pair<string, Image*>("gui_dropdown_arrrow", new Image("gfx/gui/dropdown_arrrow.png", False, 3)));
*/
}


Media::~Media()
{

    for(map<string, Image* >::iterator it = gfx.begin(); it != gfx.end(); ++it)
    {
        if(it->second == NULL)
            continue;
        delete it->second;
    }

    for(map<string, Font* >::iterator it = fonts.begin(); it != fonts.end(); ++it)
    {
        if(it->second == NULL)
            continue;
        delete it->second;
    }

}
