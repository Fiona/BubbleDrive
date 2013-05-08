/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

/**
   State object source file.
 */


// Includes
#include "State.h"
#include "../core/Game.h"
#include "../game/Ship.h"
#include "../game/Test.h"
#include "../text/Text.h"


/**
 * Constructor
 */
State::State()
{

    eGame_State = GAME_STATE_DEFAULT;
    Change_State(GAME_STATE_IN_GAME);

}


/**
 * Deconstructor
 */
State::~State()
{
}


/**
 * Switch to a new state, cleaning up anything and creating new objects.
 * @param Game_State The game state to switch to.
 */
void State::Change_State(Game_State New_State)
{

    // Clean up logo
    if(eGame_State == GAME_STATE_LOGO)
    {
        // . . .
    }

    // Clean up main game
    if(eGame_State == GAME_STATE_IN_GAME)
    {
        // . . .
    }

    eGame_State = New_State;

    // Create logo
    if(eGame_State == GAME_STATE_LOGO)
    {
        // . . .
    }

    // Create main game
    if(eGame_State == GAME_STATE_IN_GAME)
    {		
		oShip = new Ship();
		Game* game = Game::Instance();
		new Test(1);
		new Test(2);
		new Test(3);
    }

}

