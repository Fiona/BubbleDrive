/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started May 2012 by Fiona ****
 ***************************************/

/**
 * GlyphMap object source file.
 */


// Includes
#include "GlyphMap.h"
#include "consts.h"


/**
 * Contstructor. Creates blank texture and initialises it's rectangle bin.
 */
GlyphMap::GlyphMap()
{

    glGenTextures(1, &iTexture);
    glBindTexture(GL_TEXTURE_2D, iTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GLYPH_MAP_WIDTH, GLYPH_MAP_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	oBin = new RectangleBin(GLYPH_MAP_WIDTH, GLYPH_MAP_HEIGHT);

}



/**
 * Deconstructor
 */
GlyphMap::~GlyphMap()
{
	delete(oBin);
	glDeleteTextures(1, &iTexture);
}


/**
 * Adds a bitmap to the GlyphMap texture at the specified node.
 */
void GlyphMap::Add_Bitmap(unsigned char* pixels, RectangleBin::Node* node)
{

	glBindTexture(GL_TEXTURE_2D, iTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, node->iX, node->iY,
		node->iWidth, node->iHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels); 

}



/**
 * Wraps the RectangleBin Insert method.
 */
RectangleBin::Node* GlyphMap::Insert(int w, int h)
{
	return oBin->Insert(w, h);
}