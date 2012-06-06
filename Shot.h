/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _SHOT_H_
#define _SHOT_H_

/**
 * Shot object header
 */


// Includes
#include "Entity.h"


/**
 *
 */
class Shot : Entity
{

private:
    
public:

    int iRotation_To;

    Shot(float x, float y, int rot);
    void Logic();

};

#endif
