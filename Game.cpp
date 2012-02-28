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
    oState = NULL;

    oDefault_Texture_Coords.resize(8);
    oDefault_Texture_Coords[0] = 1.0f;
    oDefault_Texture_Coords[1] = 1.0f;
    oDefault_Texture_Coords[3] = 1.0f;
    oDefault_Texture_Coords[4] = 1.0f;

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
    oState = new State;

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

    std::vector<Entity*> copy_list(Registered_Entities);

    for(std::vector<Entity*>::iterator it = copy_list.begin(); it != copy_list.end(); ++it)
    {

        if(*it == NULL)
            continue;

        (*it)->Logic();

    }

}


/**
 * Draws each entity to the screen
 */
void Game::Render()
{

    oWindow->SetActive();

    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up pre-drawing
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, &oDefault_Texture_Coords[0]);

    std::vector<Entity*> copy_list(Registered_Entities);

    for(std::vector<Entity*>::iterator it = copy_list.begin(); it != copy_list.end(); ++it)
    {

        if(*it == NULL)
            continue;

        (*it)->Draw();

    }

    // Disable gl stuff
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

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

    for(std::vector<Entity*>::iterator it = Registered_Entities.begin(); it != Registered_Entities.end(); ++it)
    {
        if(*it == NULL)
            continue;
        delete(*it);
    }

    Registered_Entities.clear();

}


/**
 * Makes sure the game is keeping track of an Entity, so it
 * will happily call logic and draw methods when appropriate.
 * Is automatically called by Entity constructors.
 */
void Game::Register_Entity(Entity* Entity_To_Register)
{

    Registered_Entities.push_back(Entity_To_Register);

}


// Static initialisers
Game* Game::oInst = NULL;
