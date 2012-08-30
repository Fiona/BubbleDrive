/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _TEXT_CHARACTER_H_
#define _TEXT_CHARACTER_H_


// Includes
#include "Glyph.h"
#include "GlyphMap.h"


/**
 * Text objects keep track of the characters that 
 * need to be drawn so it's easy to reference them when
 * creating the drawing Batches.
 * These objects represent those characters.
 */
class TextCharacter
{

public:
	Glyph* oGlyph;
	GlyphMap* oGlyph_Map;
	float fX_Pos;
	float fY_Pos;

	TextCharacter(Glyph* glyph, float x_pos, float y_pos, GlyphMap* glyph_map);

};

#endif