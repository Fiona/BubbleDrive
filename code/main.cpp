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
#include "core/Game.h"


/**
 * App entry function.
 */
int main(int argc, char* argv[])
{

    return Game::Instance()->Start();

}


// poor windows. it has very specific needs.
// okay windows. i will spoon feed you.
#if defined(_WIN32)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	return main(__argc, __argv);
}
#endif