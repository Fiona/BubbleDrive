/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

/**
   Game object source file. Is the centre of the universe.
 */


// Includes
#include "consts.h"
#include "Game.h"


/**
 * Constructor
 */
Game::Game()
{

    bRunning = false;
    oGame_Time = new sf::Clock;
    oNext_Game_Tick = new sf::Time;
    *oNext_Game_Tick += sf::Milliseconds(oGame_Time->GetElapsedTime().AsMilliseconds());
    oFrame_Count_Time = new sf::Time;
    *oFrame_Count_Time += sf::Milliseconds(oGame_Time->GetElapsedTime().AsMilliseconds());
    oWindow = new sf::Window;
    iLoops_This_Frame = 0;
    iFrames_This_Second = 0;
    iCurrent_FPS = 0;
    oState = new State;

}


/**
 * Singleton implementation
 * @return Game Reference to the singleton instance.
 */
Game* Game::Instance()
{

    if(!oInst)
        oInst = new Game;
    return oInst;

}


/**
 * The core game loop. Will initialise the window and media
 * objects, then start the loop which handles input, rendering and
 * proper loop timing, sticking to a desired FPS.
 * @return int 0
 */
int Game::Start()
{

    Initialise_Window();
    Load_Media();

    bRunning = true;

    while(bRunning)
    {

        iLoops_This_Frame = 0;

        while(oGame_Time->GetElapsedTime() > *oNext_Game_Tick && iLoops_This_Frame < MAX_FRAMESKIP)
        {

            Tick();
            *oNext_Game_Tick += sf::Milliseconds(TICK_SKIP_TIME);
            iLoops_This_Frame++;

        }
              
        iFrames_This_Second++;

        if(oGame_Time->GetElapsedTime() - *oFrame_Count_Time > sf::Milliseconds(1000))
        {

            iCurrent_FPS = iFrames_This_Second;
            delete(oFrame_Count_Time);
            oFrame_Count_Time = new sf::Time;
            *oFrame_Count_Time += sf::Milliseconds(oGame_Time->GetElapsedTime().AsMilliseconds());
            iFrames_This_Second = 0;

        }

        Render();
             
    }

    Shutdown();

    return 0;

}


/**
 * Creates the window and inits some OpenGL context stuff
 */
void Game::Initialise_Window()
{

    oWindow->Create(
        sf::VideoMode(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 32),
        WINDOW_TITLE,
        sf::Style::Titlebar | sf::Style::Close
        );

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}


/**
 * Creates the Media object
 */
void Game::Load_Media()
{

    oMedia = new Media;

}


/**
 * Calls the logic for each game entity
 */
void Game::Tick()
{


}


/**
 * Draws each entity to the screen
 */
void Game::Render()
{

    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Flip
    oWindow->Display();

}


/**
 * Cleans up stuff
 */
void Game::Shutdown()
{

    oWindow->Close();

    delete(oWindow);
    delete(oGame_Time);
    delete(oNext_Game_Tick);
    delete(oFrame_Count_Time);
    delete(oState);

}


// Static initialisers
Game* Game::oInst = NULL;
