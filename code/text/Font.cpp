/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started May 2012 by Fiona ****
 ***************************************/

/**
 * Font source file.
 */


// Includes
#include "Font.h"
#include "../core/Game.h"
#include "../core/consts.h"


/**
 * Contstructor. Takes a file name and attempts to load the face in.
 */
Font::Font(std::string filename)
{

	sFilename = PATH_RESOURCES + SEPARATOR + filename;

	int error = FT_New_Face(
		Game::Instance()->oFont_Manager->oFTLib,
		sFilename.c_str(), 0, &oFace);

	if(error)
	{
		// Fail
	}

}



/**
 * Deconstructor
 */
Font::~Font()
{
	FT_Done_Face(oFace);
}