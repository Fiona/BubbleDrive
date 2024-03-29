/****************************
 BUBBLE DRIVE
 2011/2012 STOMPY BLONDIE GAMES
 ****************************
 Main app file
 ****************************/

#include <iostream>
#include <vector>
#include <algorithm> 
using namespace std;

#include "main.h"


map <string, Main_App::FuncGetter> Main_App::draw_strategies;


Main_App* Main_App::inst = NULL;

Main_App* Main_App::Instance()
{
    if(!inst)
        inst = new Main_App;
    return inst;
}


Main_App::Main_App()
{

    surf_display = NULL;
    running = True;
    desired_fps = 60;
    delay_ticks = 0;
    current_fps = 0;
    process_count = 0;
    frames_rendered = 0;
    python_interface = NULL;
    mouse = NULL;
    global_scale = 0.5f;

    // Get the application data path depending on system
#if __LINUX__
    path_application_data = getenv("HOME");
    path_application_data += "/.bubbledrive";
#elif __APPLE__
    path_application_data = getenv("HOME");
    path_application_data += "/Library/Application Support";
    path_application_data += "/BubbleDrive";
#elif __WIN32__
    TCHAR path[MAX_PATH];
    if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path)))
		path_application_data = string(path);
    path_application_data += "\\StompyBlondie";
    if(!boost::filesystem::exists(path_application_data.c_str()) || !boost::filesystem::is_directory(path_application_data.c_str()))
        boost::filesystem::create_directory(path_application_data.c_str());

    path_application_data += "\\BubbleDrive";
#endif

    if(!boost::filesystem::exists(path_application_data.c_str()) || !boost::filesystem::is_directory(path_application_data.c_str()))
        boost::filesystem::create_directory(path_application_data.c_str());

    // Deal with loading default settings etc
    path_settings_file = path_application_data + SEPARATOR + FILE_SETTINGS;
    settings = new Settings(path_settings_file);
    
    // Set up these game related structures
    world_objects_by_faction.resize(5);

}

 
int Main_App::On_Execute()
{

	Py_NoSiteFlag = 1;
    Py_Initialize();

    if(On_Init() == False)
        return -1;
 
    SDL_Event event;

    desired_fps = 60;
    frames_rendered = 0;
    current_fps = 0;
    time_taken_this_frame = 0;

    while(running)
    {

        // Empty keyboard keys released vector. it's only relevant once a frame.
        Keyboard_keys_released.clear();
        
        // Reset mouse states for these
        mouse->left_up = False;
        mouse->right_up = False;
        mouse->middle_up = False;
        mouse->x_rel = 0.0f;
        mouse->y_rel = 0.0f;
        mouse->wheel_up = False;
        mouse->wheel_down = False;

        while(SDL_PollEvent(&event))
            On_Event(&event);

        // Clean up
        minimap_objects.clear();

        // Game loop
        On_Loop();
        Do_Process_Clean();
        On_Render();

        frames_rendered++;
        process_count = Process::Process_List.size();
        
        Wait_till_next_frame();
        
    }

    On_Cleanup();
 
    return 0;

}



void Main_App::Do_Process_Clean()
{
    vector<Process*>::iterator it2;
    vector<Process*>::iterator it3;
    for(std::vector<Process*>::iterator it = Process::Processes_to_kill.begin(); it != Process::Processes_to_kill.end(); ++it)
    {
        it2 = std::find(Process::Process_List.begin(), Process::Process_List.end(), *it);
        it3 = std::find(Process::Priority_List.begin(), Process::Priority_List.end(), *it);
        if(it2 != Process::Process_List.end())
            it2 = Process::Process_List.erase(it2);
        if(it3 != Process::Priority_List.end())
            it3 = Process::Priority_List.erase(it3);
    }
    Process::Processes_to_kill.clear();
}


void Main_App::Quit()
{
    this -> running = False;
}



bool Main_App::Keyboard_key_down(SDLKey Key)
{
    return Keyboard_key_down((int)Key);
}

bool Main_App::Keyboard_key_down(int Key)
{

    vector<int>::iterator it = std::find(Keyboard_keys_down.begin(), Keyboard_keys_down.end(), Key);
    if(it != Keyboard_keys_down.end())
        return True;      
    return False;

}


bool Main_App::Keyboard_key_released(SDLKey Key)
{
    return Keyboard_key_released((int)Key);
}

bool Main_App::Keyboard_key_released(int Key)
{

    vector<int>::iterator it = std::find(Keyboard_keys_released.begin(), Keyboard_keys_released.end(), Key);
    if(it != Keyboard_keys_released.end())
        return True;      
    return False;

}


void Main_App::Wait_till_next_frame()
{

    time_taken_this_frame += SDL_GetTicks() - delay_ticks;

    if(time_taken_this_frame > 1000)
    {
        if(frames_rendered < desired_fps)
            current_fps = frames_rendered;
        else
            current_fps = desired_fps;
        frames_rendered = 0;
        time_taken_this_frame = 0;
    }

    if((SDL_GetTicks() - delay_ticks) < 1000 / desired_fps)
        SDL_Delay((1000 / desired_fps) - (SDL_GetTicks() - delay_ticks));

    delay_ticks = SDL_GetTicks();

}


Settings::Settings(){ }

Settings::Settings(string _filename)
{

    filename = _filename;

    using boost::property_tree::ptree;
    ptree pt;

    try
    {
        read_json(filename, pt);
    }
    catch(std::exception &e)
    {
        pt.put("screen_width", DEFAULT_SETTING_SCREEN_WIDTH);
        pt.put("screen_height", DEFAULT_SETTING_SCREEN_HEIGHT);
        pt.put("full_screen", DEFAULT_SETTING_FULL_SCREEN);
        pt.put("key_ship_forward", DEFAULT_SETTING_KEY_SHIP_FORWARD);
        pt.put("key_ship_back", DEFAULT_SETTING_KEY_SHIP_BACK);
        pt.put("key_ship_left", DEFAULT_SETTING_KEY_SHIP_LEFT);
        pt.put("key_ship_right", DEFAULT_SETTING_KEY_SHIP_RIGHT);
        pt.put("key_zoom_in", DEFAULT_SETTING_KEY_ZOOM_IN);
        pt.put("key_zoom_out", DEFAULT_SETTING_KEY_ZOOM_OUT);
		write_json(filename, pt);
    }

    screen_width = pt.get<float>("screen_width");
    screen_height = pt.get<float>("screen_height");
    full_screen = pt.get<bool>("full_screen");
    key_ship_forward = pt.get<int>("key_ship_forward");
    key_ship_back = pt.get<int>("key_ship_back");
    key_ship_left = pt.get<int>("key_ship_left");
    key_ship_right = pt.get<int>("key_ship_right");
    key_zoom_in = pt.get<int>("key_zoom_in");
    key_zoom_out = pt.get<int>("key_zoom_out");

}


bool Settings::save()
{

    using boost::property_tree::ptree;
    ptree pt;
    pt.put("screen_width", screen_width);
    pt.put("screen_height", screen_height);
    pt.put("full_screen", full_screen);
    pt.put("key_ship_forward", key_ship_forward);
    pt.put("key_ship_back", key_ship_back);
    pt.put("key_ship_left", key_ship_left);
    pt.put("key_ship_right", key_ship_right);
    pt.put("key_zoom_in", key_zoom_in);
    pt.put("key_zoom_out", key_zoom_out);
    try
    {
        write_json(filename, pt);
        return True;
    }
    catch(std::exception &e)
    {
        return False;
    }

}


void Mouse::set_pos(int x_pos, int y_pos)
{
    SDL_WarpMouse(x_pos, y_pos);
}


bool hasattr(boost::python::object obj, std::string const &attr_name)
{
     return PyObject_HasAttrString(obj.ptr(), attr_name.c_str());
} 


boost::mt19937 gen(std::time(0));

int randint(int min, int max)
{
    boost::random::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}


/**
 * -----------------------
 * Handy dandy mathematics
 * -----------------------
 */

/**
 * Returns the direct distance between two points.
 */
float Main_App::get_distance(float xa, float ya, float xb, float yb)
{
    return sqrt(
        (pow((yb - ya), 2) + pow((xb - xa), 2))
        );
}


/**
 * Take x/y of two points and returns the angle between those in degrees
 */
int Main_App::angle_between_points(float xa, float ya, float xb, float yb)
{
    return rad_to_deg(atan2(yb - ya, xb - xa));
}


/**
 * Returns an equivalent angle value between 0 and 360
 */
int Main_App::normalise_angle(int angle)
{
    return angle % 360;
}


/**
 * Returns the angle to turn by to get from start_angle to end_angle.
 * The sign of the result indicates the direction in which to turn.
 */
int Main_App::angle_difference(int start, int goal)
{
    start = normalise_angle(start);
    goal = normalise_angle(goal);
        
    int difference = goal - start;
    if(difference > 180.0f)
        difference -= 360.0f;
    if(difference < -180.0f)
        difference += 360.0f;
            
    return difference;
}


/**
 * Returns an angle which has been moved from 'start' closer to 
 * 'goal' by 'int'. increment should always be positive, as 
 * angle will be rotated in the direction resulting in the shortest 
 * distance to the target angle.
 */
int Main_App::near_angle(int start, int goal, int inc)
{

    start = normalise_angle(start);
    goal = normalise_angle(goal);
    float difference = angle_difference(start, goal);
            
    if(fabs(difference) <= 0)
        return start;

    if(fabs(difference) < inc)
        return goal;

    float dir = difference / fabs(difference);
    return start + (inc * (int)dir);

}


/**
 * Handy method for transforming coords from screen to world.
 */
tuple<float, float> Main_App::screen_to_world(float x, float y)
{
    return tuple<float, float>(
        camera_x + (x - (settings->screen_width/2)) / global_scale,
        camera_y + (y - (settings->screen_height/2)) / global_scale
        );
}


/**
 * Handy method for transforming coords from world to screen.
 */
tuple<float, float> Main_App::world_to_screen(float x, float y)
{
    return tuple<float, float>(
        (x - camera_x) * global_scale + (settings->screen_width/2),
        (y - camera_y) * global_scale + (settings->screen_height/2)
        );
}


/**
 * The world (pixel) coords are different to the locational coords in-universe, fiction wise. This translates.
 */
tuple<int, int> Main_App::world_to_in_universe_coords(float x, float y)
{
    return tuple<int, int>((int)(x / 1000), (int)(y / 1000));
}          


/**
 *
 */
tuple<float, float> Main_App::rotate_point(float x, float y, float rotation)
{
    rotation = deg_to_rad(rotation);
    return tuple<float, float>(
        cos(rotation) * x - sin(rotation) * y,
        sin(rotation) * x + cos(rotation) * y
        );
}

/**
 *
 */
tuple<float, float> Main_App::rotate_point_about_point(float x, float y, float rotation, float rotate_about_x, float rotate_about_y)
{
    tuple<float, float> p = Main_App::rotate_point(x - rotate_about_x, y - rotate_about_y, rotation);
    return tuple<float, float>(p.get<0>() + rotate_about_x, p.get<1>() + rotate_about_y);
}


int main(int argc, char* argv[])
{

    Main_App* app = Main_App::Instance();
    return app->On_Execute();

}
