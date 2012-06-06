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
#include "Game.h"


/**
 * Contstructor. Takes a file name and attempts to load the face in.
 */
Font::Font(std::string filename)
{

	sFilename = filename;

	int error = FT_New_Face(
		Game::Instance()->oFont_Manager->oFTLib,
		filename.c_str(), 0, &oFace);

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