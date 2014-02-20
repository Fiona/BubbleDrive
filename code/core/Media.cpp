/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

/**
   Media object source file.
 */


// Includes
#include "Media.h"
#include "consts.h"


/**
 * Constructor
 */
Media::Media()
{

    mImages.insert(std::pair<std::string, Image*>("ship", new Image("gfx" + SEPARATOR + "ships" + SEPARATOR + "ship1.png", 1)));
    mImages.insert(std::pair<std::string, Image*>("stresseffect", new Image("gfx" + SEPARATOR + "stresseffect.png", 1)));
    mImages.insert(std::pair<std::string, Image*>("laser1", new Image("gfx" + SEPARATOR + "weapons" + SEPARATOR + "laser1.png", 2)));
    mImages.insert(std::pair<std::string, Image*>("shader_shockwave", new Image("gfx" + SEPARATOR + "shader" + SEPARATOR + "shockwave.png", 1)));
    mImages.insert(std::pair<std::string, Image*>("empty", new Image("gfx" + SEPARATOR + "empty.png", 1)));

    mFonts.insert(std::pair<std::string, Font*>("test", new Font("fnt" + SEPARATOR + "aurulent.ttf")));
    mFonts.insert(std::pair<std::string, Font*>("boris", new Font("fnt" + SEPARATOR + "borisblackbloxx.ttf")));

}


/**
 * Deconstructor
 */
Media::~Media()
{

    for(std::map<std::string, Image* >::iterator it = mImages.begin(); it != mImages.end(); ++it)
    {
        if(it->second == NULL)
            continue;
        delete it->second;
    }

    for(std::map<std::string, Font* >::iterator it = mFonts.begin(); it != mFonts.end(); ++it)
    {
        if(it->second == NULL)
            continue;
        delete it->second;
    }

}

