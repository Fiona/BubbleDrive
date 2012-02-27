/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#ifndef _ENTITY_H_
#define _ENTITY_H_

/**
 * Generic game Entity object header
 */


// Includes
#include "Game.h"


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
    
public:
    float fX;
    float fY;
    int iZ;
    Image* oImage;
    Game* oGame;

    Entity();
    ~Entity();

};

#endif
