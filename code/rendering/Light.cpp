/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

/**
   Light object source file.
 */

// Includes
#include "Light.h"
#include "../core/Game.h"



/**
 * Constructor
 */
Light::Light()
{

    fX = 0.0f;
    fY = 0.0f;
	iSize = 128;
    fAlpha = 1.0f;
    aColour.resize(3, 1.0f);
    oGame = Game::Instance();
    oGame->oRenderer->Register_Light(this);

}


/**
 * Deconstructor
 */
Light::~Light()
{
    oGame->oRenderer->Remove_Light(this);
}


/**
 * Basic Getters/Setters
 */
void Light::Set_X(float X)
{
    fX = X;
}

float Light::Get_X()
{
	return fX;
}

void Light::Set_Y(float Y)
{
    fY = Y;
}

float Light::Get_Y()
{
	return fY;
 }

void Light::Set_Size(int size)
{
    iSize = size;
}

int Light::Get_Size()
{
    return iSize;
}

void Light::Set_Colour(float r, float g, float b)
{
    aColour[0] = r;
    aColour[1] = g;
    aColour[2] = b;
}

std::vector<float> Light::Get_Colour()
{
    return aColour;
}

void Light::Set_Alpha(float alpha)
{
    fAlpha = alpha;
}

float Light::Get_Alpha()
{
    return fAlpha;
}
