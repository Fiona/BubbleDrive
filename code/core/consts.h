/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#ifndef _CONSTS_H_
#define _CONSTS_H_

/**
 * Common definitions
 */

// Includes
#include <string>
#include <math.h>

#define PI 3.14159265

#define GAME_TICK_PER_SECOND 60
// 1000 / GAME_TICK_PER_SECOND
#define TICK_SKIP_TIME 16
#define MAX_FRAMESKIP 10

//define DEFAULT_SCREEN_WIDTH 640
//define DEFAULT_SCREEN_HEIGHT 480
//define DEFAULT_SCREEN_WIDTH 1024
//define DEFAULT_SCREEN_HEIGHT 768
//define DEFAULT_SCREEN_WIDTH 1280
//define DEFAULT_SCREEN_HEIGHT 1024
#define DEFAULT_SCREEN_WIDTH 1680
#define DEFAULT_SCREEN_HEIGHT 1050
#define WINDOW_TITLE "Bubble Drive"

#define GLYPH_MAP_WIDTH 1024
#define GLYPH_MAP_HEIGHT 1024

#define OPTIMAL_SCREEN_WIDTH 1680
#define OPTIMAL_SCREEN_HEIGHT 1050
#define ANGLE_OF_VIEW 30.0f
#define MIN_ZOOM (OPTIMAL_SCREEN_WIDTH / (2 * tan(ANGLE_OF_VIEW*PI/180)))
#define MAX_ZOOM 3000

#define HOTSPOT_CENTRE 0

#define RENDER_MODE_SCREEN 1
#define RENDER_MODE_WORLD 2
#define RENDER_MODE_NO_TEXTURE_SCREEN 3
#define RENDER_MODE_NO_TEXTURE_WORLD 4

#define SHADER_PRIMARY_SCREEN 0
#define SHADER_PRIMARY_WORLD 1
#define SHADER_RENDER_LAYER 2
#define SHADER_POST_GREYSCALE 3
#define SHADER_POST_BLUR 4
#define SHADER_POST_BLUR2 5
#define SHADER_POST_LIGHTS 6
#define SHADER_POST_RIPPLE 7

#define RENDER_LAYER_WORLD 1
#define RENDER_LAYER_WORLD_LIT 2
#define RENDER_LAYER_WORLD_ABOVE_LIT 3
#define RENDER_LAYER_SCREEN 4

#define PATH_RESOURCES "resources"
#define PATH_SHADERS "shaders"

#define NUM_OBJECTS_IN_VBO 50
#define NUM_VERTEX_IN_OBJECT 6
#define NUM_ELEMENTS_PER_VERTEX 12

#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1
#define MOUSE_MIDDLE 2
#define MOUSE_EXTRA_1 3
#define MOUSE_EXTRA_2 4

// Compatibility settings
#if _WIN32
#include <windows.h>
#include <string>
#define SEPARATOR std::string("\\")
#else 
#define SEPARATOR std::string("/")
#endif

#endif
