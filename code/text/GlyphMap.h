/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started May 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _GLYPHMAP_H_
#define _GLYPHMAP_H_

// gl.h requires windows.h in Windows.
#if _WIN32
#include <windows.h>
#endif

// Includes
#include <GL/glew.h>
#include <GL/gl.h>
#include "../utils/RectangleBin.h"


// Forward declaration
class Game;


/**
 * GlyphMaps hold pointers to their relevant 
 * rectangle bins and textures that Text objects
 * will use to draw.
 */
class GlyphMap
{

public:
	GlyphMap();
	~GlyphMap();
	RectangleBin::Node* Insert(int w, int h);
	void Add_Bitmap(unsigned char* pixels, RectangleBin::Node* node);

	GLuint iTexture;

private:
	Game* oGame;
	RectangleBin* oBin;

};

#endif