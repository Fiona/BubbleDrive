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
#include <memory>
#include "../core/Entity.h"

// forward decl
class Ship;
class Vector2D;

/**
 *
 */
class Shot : Entity
{

private:
    
    Ship* Parent;

public:

	int Life;
    std::unique_ptr<Vector2D> Velocity;
    Shot(float x, float y, int rot, Ship* parent);
    void Logic();

};

#endif
