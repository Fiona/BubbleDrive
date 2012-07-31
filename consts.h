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

//define DEFAULT_SCREEN_WIDTH 320
//define DEFAULT_SCREEN_HEIGHT 240
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
#define MIN_ZOOM (OPTIMAL_SCREEN_WIDTH / (2 * tan(30.0f*PI/180)))
#define MAX_ZOOM 3000

#define HOTSPOT_CENTRE 0

#define RENDER_MODE_SCREEN 1
#define RENDER_MODE_WORLD 2
#define RENDER_MODE_NO_TEXTURE_SCREEN 3
#define RENDER_MODE_NO_TEXTURE_WORLD 4

#define PATH_SHADERS "shaders"

// Compatibility settings
#if _WIN32
#include <windows.h>
#include <string>

#define SEPARATOR std::string("\\")

#else 

#define SEPARATOR std::string("/")

#endif

#endif
