/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _SHIP_H_
#define _SHIP_H_

/**
 * Ship object header
 */


// Includes
#include "../core/Entity.h"


/**
 *
 */
class Ship : Entity
{

private:
    
public:

    Ship();
    void Logic();
    void create_vorticies(float x, float y, int type);
    int current_rotation;
    int current_rotation_2;

};

#endif
