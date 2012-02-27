/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

/**
 * Image object source file.
 */


// Includes
#include "Image.h"


/**
 * Constructor
 */
Image::Image(std::string file_name)
{

    sFile_Name = file_name;

    sf::Image* raw_image = new sf::Image;

    if(!raw_image->LoadFromFile(file_name))
        return;

    Create_From_SMFL_Image(raw_image);

    delete(raw_image);

}


void Image::Create_From_SMFL_Image(sf::Image* raw_image)
{

    iWidth = raw_image->GetWidth();
    iHeight = raw_image->GetHeight();

    glGenTextures(1, &iTexture_Num);
    glBindTexture(GL_TEXTURE_2D, iTexture_Num);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, iWidth, iHeight, GL_RGBA, GL_UNSIGNED_BYTE, raw_image->GetPixelsPtr());

}


/**
 * Deconstructor
 */
Image::~Image()
{

    if(iTexture_Num > 0)
    {
        glDeleteTextures(1, &iTexture_Num);
        iTexture_Num = 0;
    }

}

