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
    *oNext_Game_Tick += sf::milliseconds(oGame_Time->getElapsedTime().asMilliseconds());
    oFrame_Count_Time = new sf::Time;
    *oFrame_Count_Time += sf::milliseconds(oGame_Time->getElapsedTime().asMilliseconds());
    oWindow = new sf::Window;
    iLoops_This_Frame = 0;
    iFrames_This_Second = 0;
    iCurrent_FPS = 0;
    oState = NULL;

    iCurrent_Bound_Texture = -1;
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

        while(oGame_Time->getElapsedTime() > *oNext_Game_Tick && iLoops_This_Frame < MAX_FRAMESKIP)
        {

            Update_Events();
            Tick();
            *oNext_Game_Tick += sf::milliseconds(TICK_SKIP_TIME);
            iLoops_This_Frame++;

        }
              
        iFrames_This_Second++;

        if(oGame_Time->getElapsedTime() - *oFrame_Count_Time > sf::milliseconds(1000))
        {

            iCurrent_FPS = iFrames_This_Second;
            delete(oFrame_Count_Time);
            oFrame_Count_Time = new sf::Time;
            *oFrame_Count_Time += sf::milliseconds(oGame_Time->getElapsedTime().asMilliseconds());
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

    oWindow->create(
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
 * Called before each game Tick. Empties the current event queue and sets
 * them up to be queried by entities in the game.
 */
void Game::Update_Events()
{

    aKeys_Released.clear();

    sf::Event Event;
    while(oWindow->pollEvent(Event))
    {

        if(Event.type == sf::Event::KeyReleased)
            aKeys_Released.push_back(Event.key.code);

    }

}


/**
 * Calls the logic for each game entity
 */
void Game::Tick()
{

    std::cout << iCurrent_FPS << std::endl;

    std::vector<Entity*> copy_list(Registered_Entities);

    for(std::vector<Entity*>::iterator it = copy_list.begin(); it != copy_list.end(); ++it)
    {

        if(*it == NULL)
            continue;

        (*it)->Logic();

    }

    if(Entities_To_Delete.size() > 0)
    {
        for(std::vector<Entity*>::iterator it = Entities_To_Delete.begin(); it != Entities_To_Delete.end(); ++it)
        {
            if(*it == NULL)
                continue;
            delete(*it);
        }
        Entities_To_Delete.clear();
    }

    if(Keyboard_Key_Released(sf::Keyboard::Escape))
        bRunning = false;

}


/**
 * Draws each entity to the screen
 */
void Game::Render()
{

    oWindow->setActive();

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
    oWindow->display();

}


/**
 * Cleans up stuff
 */
void Game::Shutdown()
{

    oWindow->close();

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

    aKeys_Released.clear();

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


/**
 * Used to prime an entity for eventual removal
 */
void Game::Unregister_Entity(Entity* Entity_To_Unregister)
{

    std::vector<Entity*>::iterator it;
    it = std::find(Registered_Entities.begin(), Registered_Entities.end(), Entity_To_Unregister);
    if(it != Registered_Entities.end())
        Registered_Entities.erase(it);

    Entities_To_Delete.push_back(Entity_To_Unregister);

}


/**
 * Returns true/false if a particular key is currently being held down or not.
 * Should be passed key codes from the sf::Keyboard enum.
 */
bool Game::Keyboard_Key_Down(sf::Keyboard::Key k)
{
    return sf::Keyboard::isKeyPressed(k);
}


/**
 * Returns true/false if a particular key was released the previous tick.
 * Should be passed key codes from the sf::Key enum.
 */
bool Game::Keyboard_Key_Released(sf::Keyboard::Key k)
{

    std::vector<sf::Keyboard::Key>::iterator it;
    it = std::find(aKeys_Released.begin(), aKeys_Released.end(), k);
    if(it != aKeys_Released.end())
        return true;
    return false;

}


/**
 * Converts degrees to radians
 */
float Game::Deg_To_Rad(float degrees)
{
    return (3.1415926f / 180.0f) * degrees;
}


/**
 * Converts radians to degrees
 */
float Game::Rad_To_Deg(float radians)
{
    return radians * 180.0f / 3.1415926f;
}


// Static initialisers
Game* Game::oInst = NULL;
