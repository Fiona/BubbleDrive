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

	iGlyph_Index = FT_Get_Char_Index(font->oFace, (int)*glyph.c_str());

	FT_Set_Char_Size(font->oFace, 0, size*64, 0, 0);
	FT_Load_Char(font->oFace, (int)*glyph.c_str(), FT_LOAD_RENDER);

	fWidth = font->oFace->glyph->metrics.width / 64.0f;
	fHeight = font->oFace->glyph->metrics.height / 64.0f;
	fAdvance = font->oFace->glyph->metrics.horiAdvance / 64.0f;
	fY_Bearing = font->oFace->glyph->metrics.horiBearingY / 64.0f;

}



/**
 * Deconstructor
 */
Glyph::~Glyph()
{

}


/**
 * Passing in another Glyph, this will return the kerning distance
 * between the two Glyphs as a float.
 */
float Glyph::Get_Kerning(Glyph* other_glyph)
{
	if(this->oFont->oFace != other_glyph->oFont->oFace)
		return 0.0f;
	FT_Vector delta;
	FT_Get_Kerning(this->oFont->oFace, other_glyph->iGlyph_Index, this->iGlyph_Index, FT_KERNING_DEFAULT, &delta);
	return (float)(delta.x / 64.0f);
}