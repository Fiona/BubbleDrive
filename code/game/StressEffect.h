/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _STRESSEFFECT_H_
#define _STRESSEFFECT_H_

/**
 * Stress effect object header
 */


// Includes
#include "../core/Entity.h"


/**
 *
 */
class StressEffect : Entity
{

private:
    
public:

    int iRotation_To;
    StressEffect(float x, float y, int rot);
    void Logic();
	void Kill();

};

#endif
