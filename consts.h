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


#define GAME_TICK_PER_SECOND 60
// 1000 / GAME_TICK_PER_SECOND
#define TICK_SKIP_TIME 16
#define MAX_FRAMESKIP 10

#define DEFAULT_SCREEN_WIDTH 1024
#define DEFAULT_SCREEN_HEIGHT 768
#define WINDOW_TITLE "Bubble Drive"

#define GLYPH_MAP_WIDTH 512
#define GLYPH_MAP_HEIGHT 512

// Compatibility settings
#if _WIN32
#include <windows.h>
#include <string>

#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif
#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif

#define SEPARATOR std::string("\\")

#else 

#define SEPARATOR std::string("/")

#endif

#endif
