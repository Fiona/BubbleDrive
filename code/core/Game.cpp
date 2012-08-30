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
#include <fstream>
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

	aCamera_Position[0] = 0.0f; aCamera_Position[1] = 0.0f; aCamera_Position[2] = 500.0; aCamera_Position[3] = 0.0f;

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

    if(!Initialise_Graphics())
	{
		Shutdown();
		return 0;
	}

    Load_Media();
    oState = new State;
	
	/**/
	oFPS_Text = new Text();
	oFPS_Text->Set_Render_Mode(RENDER_MODE_SCREEN);
	oFPS_Text->Set_Text("FPS: 0");
	oFPS_Text->Set_Font(Game::Instance()->oMedia->mFonts["test"]);
	oFPS_Text->Set_Size(50);
	oFPS_Text->Set_X(0.0f);
	oFPS_Text->Set_Y(0.0f);
	oFPS_Text->Set_Z(-10.0f);
	oFPS_Text->Ready_For_Rendering();
	/**/
	/**/
	Text* blah = new Text();
	blah->Set_Render_Mode(RENDER_MODE_WORLD);
	blah->Set_Text("I'm going to be testing new lines\nhere, that was a new line.\nAnd so is this.\n\nThat was two!\nThis is all the same Text object.");
	blah->Set_Font(Game::Instance()->oMedia->mFonts["boris"]);
	blah->Set_Size(40);
	blah->Set_X(100.0f);
	blah->Set_Y(100.0f);
	blah->Set_Line_Height_Padding(10.0f);
	blah->Ready_For_Rendering();
	/**/

    bRunning = true;

    while(bRunning)
    {

		/**/
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

			oFPS_Text->Set_Text("fps: " + boost::lexical_cast<std::string>(iCurrent_FPS));
			oFPS_Text->Ready_For_Rendering();

        }		

        Render();
		Cleanup_Deleted_Entities();
		/**/

		if(Keyboard_Key_Down(sf::Keyboard::LAlt))
		{

			std::cout << iCurrent_FPS << std::endl;

		}

		/**
		Update_Events();
		Tick();
        Render();
		Cleanup_Deleted_Entities();
		/**/

    }

    Shutdown();

    return 0;

}


/**
 * Creates the window and inits some OpenGL context stuff
 */
bool Game::Initialise_Graphics()
{
	
	// Create the window
    oWindow->create(
        sf::VideoMode(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 32),
        WINDOW_TITLE,
        sf::Style::Titlebar | sf::Style::Close
        );

	// Initialise GL extensions
	GLenum err = glewInit();
	if(GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return false;
	}

	// Create render mode
	Setup_Batches();

	// Create OpenGL Viewport
    glViewport(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    glClearColor(0.0, 0.0, 0.0, 0);
	glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);

	return true;

}


/**
 * Used while initialising graphics to create the BatchManager
 */
void Game::Setup_Batches()
{

	oBatch_Manager = new BatchManager();

}

/**
 * Creates the Media object
 */
void Game::Load_Media()
{

	oFont_Manager = new FontManager;
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

	if(Keyboard_Key_Down(sf::Keyboard::LControl))
		aCamera_Position[2] -= 20.0f;
	if(Keyboard_Key_Down(sf::Keyboard::LShift))
		aCamera_Position[2] += 20.0f;

	if(Keyboard_Key_Down(sf::Keyboard::Comma))
		aCamera_Position[1] += 5.0f;
	if(Keyboard_Key_Down(sf::Keyboard::O))
		aCamera_Position[1] -= 5.0f;

	if(Keyboard_Key_Down(sf::Keyboard::E))
		aCamera_Position[0] -= 5.0f;
	if(Keyboard_Key_Down(sf::Keyboard::A))
		aCamera_Position[0] += 5.0f;

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

	// entity cleanup was here
    if(Keyboard_Key_Released(sf::Keyboard::Escape))
        bRunning = false;

}


/**
 * 
 */
void Game::Cleanup_Deleted_Entities()
{

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

}


/**
 * Draws each entity to the screen
 */
void Game::Render()
{

	// Make sure zoom level isn't CERAZY
	if(aCamera_Position[2] < (float)MIN_ZOOM)
		aCamera_Position[2] = (float)MIN_ZOOM;
	if(aCamera_Position[2] > (float)MAX_ZOOM)
		aCamera_Position[2] = (float)MAX_ZOOM;

    oWindow->setActive();

    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

	// Tell BatchManager to update
	oBatch_Manager->Update_And_Render_Batches();

	/*
    std::vector<Entity*> copy_list(Registered_Entities);

    for(std::vector<Entity*>::iterator it = copy_list.begin(); it != copy_list.end(); ++it)
    {

        if(*it == NULL)
            continue;

		if((*it)->bVBO_Dirty)
			(*it)->Create_VBO();

		oRender_Modes[(*it)->Get_Render_Mode()]->Setup_Render_Pass(*it);
		oRender_Modes[(*it)->Get_Render_Mode()]->Do_Render_Pass(*it);
		oRender_Modes[(*it)->Get_Render_Mode()]->Cleanup_Render_Pass(*it);

    }*/

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
	delete(oMedia);
	delete(oFont_Manager);
	delete(oBatch_Manager);

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


/**
 * Loads text from a text file and returns as string.
 */
std::string Game::Load_From_File(std::string file_name)
{
	
	std::string str, line;
	std::ifstream file;
	file.open(file_name.c_str());

	if(!file.is_open())
		return 0;

	while(!file.eof())
	{
		std::getline(file, line);
		str += line;
		str += "\n";
	}

	file.close();

	return str;

}

// Static initialisers
Game* Game::oInst = NULL;
