/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Nov 2013 by Fiona ****
 ***************************************/

#pragma once
#ifndef _SHOCKWAVE_H_
#define _SHOCKWAVE_H_


// Includes
#include <vector>

// Forward declaration
class Game;


/**
 * Shockwaves can be created, placed and their attributes 
 * like alpha and size changed. They will be rendered into the world
 * on any layer with the PostShaderRipple post-processing attached.
 */
class Shockwave
{

public:
	Shockwave();
	~Shockwave();

    void Set_X(float X);
    float Get_X();
    void Set_Y(float Y);
    float Get_Y();
    void Set_Size(int size);
    int Get_Size();
    void Set_Alpha(float alpha);
    float Get_Alpha();

private:
	Game* oGame;
    float fX;
    float fY;
	int iSize;
    float fAlpha;

};


#endif
