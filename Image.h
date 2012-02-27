/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#ifndef _IMAGE_H_
#define _IMAGE_H_

/**
 * Image object header
 */


// Includes
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SFML/Graphics/Image.hpp>


/**
 * Image object, loads image files
 */
class Image
{

private:
    std::string sFile_Name;
    GLuint iTexture_Num;

    void Create_From_SMFL_Image(sf::Image* raw_image);
    
public:
    int iWidth;
    int iHeight;

    Image(std::string file_name);
    ~Image();

};


#endif
