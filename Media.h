/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#ifndef _MEDIA_H_
#define _MEDIA_H_

/**
 * Media object header
 */


// Includes
#include <string>
#include <map>
#include "Image.h"


/**
 * Main media object, is responsible for loading/handling 
 * images, audio and fonts.
 */
class Media
{

private:

public:
    Media();
    ~Media();

    std::map <std::string, Image* > mImages;
    //std::map <string, Font* > mFonts;
    //std::map <string, Audio* > mAudio;

};


#endif
