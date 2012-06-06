/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started May 2012 by Fiona ****
 ***************************************/

/**
 * Glyph source file.
 */


// Includes
#include "Glyph.h"


/**
 * Contstructor.
 */
Glyph::Glyph(std::string glyph, Font* font, int size)
{

	sGlyph = glyph;
	oFont = font;
	iSize = size;

	FT_Set_Char_Size(font->oFace, 0, size*64, 0, 0);

	/*
	FT_Load_Glyph(
		font->oFace,
		FT_Get_Char_Index(font->oFace, (int)*glyph.c_str()),
		FT_LOAD_DEFAULT
		);
	*/
	FT_Load_Char(font->oFace, (int)*glyph.c_str(), FT_LOAD_RENDER);

	fWidth = font->oFace->glyph->metrics.width / 64.0f;
	fHeight = font->oFace->glyph->metrics.height / 64.0f;
	fAdvance = font->oFace->glyph->metrics.horiAdvance / 64.0f;
	fY_Bearing = font->oFace->glyph->metrics.horiBearingY / 64.0f;
	
	// Add to glyph bin texture
	//FT_Render_Glyph(font->oFace->glyph, FT_RENDER_MODE_NORMAL);
	

}



/**
 * Deconstructor
 */
Glyph::~Glyph()
{

}