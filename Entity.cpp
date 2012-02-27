/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

/**
   Generic Entity object source file.
 */


// Includes
#include "Entity.h"
#include "Game.h"


/**
 * Constructor
 */
Entity::Entity()
{

    fX = 0.0f;
    fY = 0.0f;
    iZ = 0;
    oGame = Game::Instance();
    oGame->Register_Entity(this);

}


/**
 * Deconstructor
 */
Entity::~Entity()
{
}
