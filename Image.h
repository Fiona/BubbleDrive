/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _IMAGE_H_
#define _IMAGE_H_

/**
 * Image object header
 */

// Windows specific
#if _WIN32
#include <windows.h>
#endif


// Includes
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SFML/Graphics/Image.hpp>


/**
 * Image object, loads image files
 */
class Image
{

private:
    std::string sFile_Name;

    void Create_From_SMFL_Image(sf::Image* raw_image);
    
public:
    int iWidth;
    int iHeight;
    int iRaw_Surface_Width;
    GLuint iTexture_Num;
    int iFrame_Count;
    float aVertex_List[12];
    std::vector< std::vector<float> > aTexture_Coord_Lists;

    Image(std::string file_name, int frame_count);
    ~Image();

};


#endif
