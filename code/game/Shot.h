/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************/

#pragma once
#ifndef _SHOT_H_
#define _SHOT_H_

/**
 * Shot object header
 */


// Includes
#include "../core/Entity.h"
#include "../utils/Vector2D.h"


/**
 *
 */
class Shot : Entity
{

private:
    
    Ship* Parent;
    Vector2D Velocity;

public:

	int Life;
    Shot(float x, float y, int rot, Ship* parent);
    void Logic();

};

#endif
