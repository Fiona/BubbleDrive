/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_

/**
 * Generic game Entity object header
 */


// Includes
#include <vector>
#include "Image.h"


// Forward declaration
class Game;


/**
 * All visible objects in the game inherit from the generic
 * Entity class. It handles basic drawing, screen position,
 * rotation etc.
 * When subclassing, the Entity constructor should always be
 * called to ensure that it gets added to the global entity list.
 */
class Entity
{

private:
    float fX;
    float fY;
    float iZ;
	float fRotation;
	float fScale;
    Image* oImage;
    int iImage_Frame;
	int iRender_Mode;
	GLfloat aVertex_Data[12];
	GLfloat aColour_Data[24];
	GLfloat aText_Coord_Data[12];

	void Create_Vertex_Array();
	void Create_Colour_Array();
	void Create_Texture_Coord_Array();

public:
    Game* oGame;
    float fAlpha;
    std::vector<float> aColour;
	GLfloat aPosition[4];
	GLuint aVBO[3];

    Entity();
    ~Entity();
    virtual void Logic();
    virtual void Draw();
    virtual void Kill();
    virtual void Set_X(float X);
    virtual float Get_X();
    virtual void Set_Y(float Y);
    virtual float Get_Y();
    virtual void Set_Z(float Z);
    virtual float Get_Z();
    virtual void Set_Rotation(float Rotation);
    virtual float Get_Rotation();
    virtual void Set_Scale(float Scale);
    virtual float Get_Scale();
    virtual void Set_Image(Image* Image);
    virtual Image* Get_Image();
    virtual void Set_Image_Frame(int image_frame);
    virtual int Get_Image_Frame();
    virtual void Set_Render_Mode(int render_mode);
    virtual int Get_Render_Mode();
    virtual void Set_Colour(float r, float g, float b);

	std::vector<float> Get_Hotspot_Pos(int spot);
    void Advance_Towards(float distance, int rot);

};

#endif
