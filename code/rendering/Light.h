/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _LIGHT_H_
#define _LIGHT_H_


// Includes
#include <vector>

// Forward declaration
class Game;


/**
 * Light objects can be created, placed and their attributes 
 * like colour and size changed. They will be rendered into the world
 * on any layer with the PostShaderLights post-processing attached.
 */
class Light
{

public:
	Light();
	~Light();

    void Set_X(float X);
    float Get_X();
    void Set_Y(float Y);
    float Get_Y();
	void Set_Rotation(float Rotation);
    float Get_Rotation();
    void Set_Size(int size);
    int Get_Size();
    void Set_Alpha(float alpha);
    float Get_Alpha();
    void Set_Colour(float r, float g, float b);
    std::vector<float> Get_Colour();

private:
	Game* oGame;
    float fX;
    float fY;
	int iSize;
    float fAlpha;
    std::vector<float> aColour;

};


#endif