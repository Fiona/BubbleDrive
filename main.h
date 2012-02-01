/****************************
 BUBBLE DRIVE
 2011/2012 STOMPY BLONDIE GAMES
 ****************************
 Main app Header
 ****************************/

#ifndef _MAIN_H_
#define _MAIN_H_

#define BOOST_PYTHON_STATIC_LIB

// Compatibility settings
// lololol microsoft on the GL_BGR thing
#if _WIN32
#include <windows.h>
#include <Shlobj.h>
#include <string>

#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif
#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif

#define SEPARATOR "\\"

#else 

#define SEPARATOR "/"

#endif

// Yeah so what?
#define True true
#define False false

// Default settings
#define DEFAULT_SETTING_SCREEN_WIDTH 1024
#define DEFAULT_SETTING_SCREEN_HEIGHT 768
#define DEFAULT_SETTING_FULL_SCREEN "0"
#define DEFAULT_SETTING_KEY_SHIP_FORWARD SDLK_w
#define DEFAULT_SETTING_KEY_SHIP_BACK SDLK_s
#define DEFAULT_SETTING_KEY_SHIP_LEFT SDLK_a
#define DEFAULT_SETTING_KEY_SHIP_RIGHT SDLK_d
#define DEFAULT_SETTING_KEY_ZOOM_IN SDLK_PAGEUP
#define DEFAULT_SETTING_KEY_ZOOM_OUT SDLK_PAGEDOWN

// Misc defines
#define FILE_SETTINGS "settings.json"
#define BACKGROUND_NUM_NEBULA_TYPES 3
#define BACKGROUND_NUM_NEBULA_ITEMS 8

// STD and boost includes
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include "boost/foreach.hpp"
#include "boost/tuple/tuple.hpp"
#include "boost/format.hpp"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

// GL and SDL
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_ttf.h>

// Program includes
#include "image.h"
#include "font.h"
#include "process.h"
#include "media.h"
#include "python_interface.h"

class Mouse;
class Settings;


/*
 */
class Main_App
{
 
private:
    bool running;
    SDL_Surface* surf_display;
    int desired_fps;
    int delay_ticks;
    int frames_rendered;
    int time_taken_this_frame;
    Python_Interface* python_interface;

    void Wait_till_next_frame();


public:
    typedef void (Process::*FuncGetter)();
    static map <string, FuncGetter> draw_strategies;

    Settings* settings;
    Media* media;
    std::vector<SDLKey> Keyboard_keys_down;
    std::vector<SDLKey> Keyboard_keys_released;
    int current_fps;
    int process_count;
    string path_application_data;
    string path_settings_file;

    Mouse* mouse;

    Main_App();
    void Quit();
    int On_Execute();
    bool On_Init();   
    void On_Event(SDL_Event* Event);    
    void On_Loop();    
    void On_Render();    
    void Do_Process_Clean();
    void On_Cleanup(); 
    bool Keyboard_key_down(SDLKey Key);
    bool Keyboard_key_released(SDLKey Key);

    // Game specific
    float camera_x;
    float camera_y;
    float global_scale;

    tuple<float, float> screen_to_world(float x, float y);
    tuple<float, float> world_to_screen(float x, float y);
    tuple<int, int> world_to_in_universe_coords(float x, float y);

};


/*
 */
class Mouse
{
public:
    float x;
    float y;
    float x_rel;
    float y_rel;
    bool left_down;
    bool left_up;
    bool right_down;
    bool right_up;
    bool middle_down;
    bool middle_up;
    bool wheel_down;
    bool wheel_up;

    void set_pos(int x_pos, int y_pos);
};


/*
 */
class Settings
{
public:
    Settings();
    Settings(string _filename);

    string filename;
    float screen_width;
    float screen_height;
    bool full_screen;
    int key_ship_forward;
    int key_ship_back;
    int key_ship_left;
    int key_ship_right;
    int key_zoom_in;
    int key_zoom_out;

    bool save();
};


bool hasattr(boost::python::object obj, std::string const &attr_name);

int randint(int min, int max);

#endif
