/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _STATE_H_
#define _STATE_H_

/**
 * State object header
 */


// Includes
// ...

// forward decl
class Ship;

/**
 * Acceptable states are defined here
 */
enum Game_State
{
    GAME_STATE_DEFAULT,
    GAME_STATE_LOGO,
    GAME_STATE_IN_GAME
};


/**
 * The State object is a global state machine for the game.
 * As well as changing game states, it keeps track of objects and lists
 * that other game objects might need to get access to.
 * Only one State instance should be created, as a member of the Game singleton.
 */
class State
{

private:
    
public:
    Game_State eGame_State;
	Ship* oShip;

    State();
    ~State();
    void Change_State(Game_State New_State);

};

#endif
