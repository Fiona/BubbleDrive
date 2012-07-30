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
Image::Image(std::string file_name, int frame_count)
{

    sFile_Name = file_name;

    sf::Image* raw_image = new sf::Image;

    if(!raw_image->loadFromFile(file_name))
        return;

    Create_From_SMFL_Image(raw_image);

    delete(raw_image);

    iFrame_Count = frame_count;
    iRaw_Surface_Width = iWidth;
    if(iFrame_Count > 1)
        iWidth = iWidth / iFrame_Count;

    // Create vertex list
    for(int i = 0; i < 12; i++)
        aVertex_List[i] = 0.0f;
    aVertex_List[0] = (float)iWidth;
    aVertex_List[1] = (float)iHeight;
    aVertex_List[4] = (float)iHeight;
    aVertex_List[6] = (float)iWidth; 

    // Create texture coord lists for each frame
    float texture_x_from;
    float texture_x_to;
    float sequence_start_pos;

    if(iFrame_Count > 1)
        sequence_start_pos = 0.01f * (((float)iWidth / (float)iRaw_Surface_Width) * 100.0f);
    else
        sequence_start_pos = 0.0f;

    aTexture_Coord_Lists = std::vector< std::vector<float> >(iFrame_Count, std::vector<float>(8, 0.0f));

    for(int frame = 0; frame < iFrame_Count; frame++)
    {

        if(iFrame_Count == 1)
        {
            texture_x_from = 0.0f;
            texture_x_to = 1.0;
        }
        else
        {
            texture_x_from = sequence_start_pos * frame;
            texture_x_to = (sequence_start_pos *frame) + sequence_start_pos;
        }

        aTexture_Coord_Lists[frame][0] = texture_x_to; aTexture_Coord_Lists[frame][1] = 1.0f;
        aTexture_Coord_Lists[frame][2] = texture_x_from; aTexture_Coord_Lists[frame][3] = 1.0f;
        aTexture_Coord_Lists[frame][4] = texture_x_to;
        aTexture_Coord_Lists[frame][6] = texture_x_from;

    }

}


void Image::Create_From_SMFL_Image(sf::Image* raw_image)
{

	iWidth = raw_image->getSize().x;
	iHeight = raw_image->getSize().y;

    glGenTextures(1, &iTexture_Num);
    glBindTexture(GL_TEXTURE_2D, iTexture_Num);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, iWidth, iHeight, GL_RGBA, GL_UNSIGNED_BYTE, raw_image->getPixelsPtr());

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

