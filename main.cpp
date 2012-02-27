/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

/**
 * Main source file, just inits and starts the game object.
 */


// Includes
#include "Game.h"


/**
 * App entry function.
 */
int main(int argc, char* argv[])
{

    return Game::Instance()->Start();

}
