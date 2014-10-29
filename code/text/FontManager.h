/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started May 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_


// Includes
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/foreach.hpp>


// forward decl
class GlyphMap;
class Glyph;
class Font;

// Short hand for the keys for the map of Glyphs
typedef boost::tuple<Font*, int, std::string> GlyphKey;

/**
 * The Font Manager is responsible for keeping global lists of font
 * glyphs and the maps they're stored in.
 * It also holds the freetype2 library instance.
 */
class FontManager
{

public:
	FontManager();
	~FontManager();

	Glyph* Request_Glyph(Font* font, int size, std::string character);
	bool Place_Glyph_In_GlyphBin(Glyph* glyph);

	std::vector<GlyphMap*> oGlyph_Maps;
	std::map<GlyphKey, Glyph*> oGlyphs;
	
	FT_Library oFTLib;

};

#endif
