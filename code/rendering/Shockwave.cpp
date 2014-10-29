/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************/

/**
   Shockwave object source file.
 */

// Includes
#include "Shockwave.h"
#include "../core/Game.h"
#include "Renderer.h"



/**
 * Constructor
 */
Shockwave::Shockwave()
{

    fX = 0.0f;
    fY = 0.0f;
	iSize = 0;
    fAlpha = 1.0f;
    oGame = Game::Instance();
    oGame->oRenderer->Register_Shockwave(this);

}


/**
 * Deconstructor
 */
Shockwave::~Shockwave()
{
    oGame->oRenderer->Remove_Shockwave(this);
}


/**
 * Basic Getters/Setters
 */
void Shockwave::Set_X(float X)
{
    fX = X;
}

float Shockwave::Get_X()
{
	return fX;
}

void Shockwave::Set_Y(float Y)
{
    fY = Y;
}

float Shockwave::Get_Y()
{
	return fY;
 }

void Shockwave::Set_Size(int size)
{
    iSize = size;
}

int Shockwave::Get_Size()
{
    return iSize;
}

void Shockwave::Set_Alpha(float alpha)
{
    fAlpha = alpha;
}

float Shockwave::Get_Alpha()
{
    return fAlpha;
}
