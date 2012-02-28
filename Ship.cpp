/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

/**
 * Ship object source file.
 */


// Includes
#include <iostream>
#include "Ship.h"


/**
 * Constructor
 */
Ship::Ship() : Entity()
{

    Set_Image(oGame->oMedia->mImages["ship"]);
    Set_X(300.0f);
    Set_Y(300.0f);

}


/**
 * 
 */
void Ship::Logic()
{
}
