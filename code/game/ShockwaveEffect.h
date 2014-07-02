/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Nov 2013 by Fiona ****
 ***************************************/

#pragma once
#ifndef _SHOCKWAVEEFFECT_H_
#define _SHOCKWAVEEFFECT_H_

/**
 * Shockwave effect object header
 */


// Includes
#include "../core/Entity.h"


/**
 *
 */
class ShockwaveEffect : Entity
{

private:
    
    float fSpeed;
    int iSize;
    int iCurrent_Size;
	Shockwave oMy_Shockwave;

public:

    ShockwaveEffect(float x, float y, float speed, int size);
    void Logic();

	void Update_My_Shockwave();

};

#endif
