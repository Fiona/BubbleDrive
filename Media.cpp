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

    mImages.insert(std::pair<std::string, Image*>("ship", new Image("gfx" + SEPARATOR + "ship.png", 2)));
    mImages.insert(std::pair<std::string, Image*>("shot", new Image("gfx" + SEPARATOR + "shot.png", 1)));

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

}

