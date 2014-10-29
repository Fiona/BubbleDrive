/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

/**
 * TextCharacter object source file.
 */

// Includes
#include "TextCharacter.h"
#include "Glyph.h"
#include "GlyphMap.h"


/**
 * Constructor, pretty much all that's required.
 */
TextCharacter::TextCharacter(Glyph* glyph, float x_pos, float y_pos, GlyphMap* glyph_map)
{

	oGlyph = glyph;
	oGlyph_Map = glyph_map;
	fX_Pos = x_pos;
	fY_Pos = y_pos;

}
