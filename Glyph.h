/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started May 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _GLYPH_H_
#define _GLYPH_H_

// Includes
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include "Font.h"
#include "RectangleBin.h"


/**
 * Glyphs hold the pointer to their relevant Node object
 * in it's GlyphBin, along with keeping track of all the information
 * about the Glyph in question.
 */
class Glyph
{

public:
	Glyph(std::string glyph, Font* font, int size);
	~Glyph();

	Font* oFont;
	int iGlyph_Map;
	RectangleBin::Node* oNode;
	float fWidth;
	float fHeight;
	float fAdvance;
	float fY_Bearing;

private:
	std::string sGlyph;
	int iSize;

};

#endif