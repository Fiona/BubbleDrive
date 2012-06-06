/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started May 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _FONT_H_
#define _FONT_H_


// Includes
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H


/**
 * Font objects are responsible for loading font
 * files with Freetype2, extracting font info and 
 * and glyph info.
 */
class Font
{

public:
	Font(std::string filename);
	~Font();

	FT_Face oFace;

private:
	std::string sFilename;

};

#endif