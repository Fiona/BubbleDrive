/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#ifndef _GAME_H_
#define _GAME_H_

/**
 * Game object header
 */


// Includes
#include <iostream>
#include <cstddef>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SFML/Window.hpp>
#include "Media.h"
#include "State.h"


/**
 * The Game object handles the basic game loop, keeps track
 * of media and keeps track of game objects among other things.
 *
 * It is a singleton, it is safe to pass a reference to this singleton
 * but is preferable to get it using the Instance() method.
 */
class Game
{

private:
    static Game* oInst;
    sf::Clock* oGame_Time;
    sf::Time* oNext_Game_Tick;
    sf::Time* oFrame_Count_Time;
    int iLoops_This_Frame;
    int iFrames_This_Second;

    void Initialise_Window();
    void Load_Media();
    void Tick();
    void Render();
    void Shutdown();

public:
    sf::Window* oWindow;
    bool bRunning;
    int iCurrent_FPS;
    Media* oMedia;
    State* oState;

    Game();
    static Game* Instance();
    int Start();

};


#endif
