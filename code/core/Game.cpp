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
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include "consts.h"
#include "Game.h"
#include "Entity.h"
#include "Media.h"
#include "State.h"
#include "../rendering/Renderer.h"
#include "../rendering/PrimaryShader.h"
#include "../text/FontManager.h"
#include "../text/Text.h"
#include "../particles/ParticleSystem.h"


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
	iRender_Frame_Time = 0;
    oState = NULL;
	aMouse_Pos.resize(2);
	aMouse_Buttons.resize(5);

    iCurrent_Bound_Texture = -1;
    oDefault_Texture_Coords.resize(8);
    oDefault_Texture_Coords[0] = 1.0f;
    oDefault_Texture_Coords[1] = 1.0f;
    oDefault_Texture_Coords[3] = 1.0f;
    oDefault_Texture_Coords[4] = 1.0f;

	aCamera_Position[0] = 0.0f; aCamera_Position[1] = 0.0f; aCamera_Position[2] = 500.0; aCamera_Position[3] = 0.0f;

	fScreen_Blur_Amount = 0.0f;
	fSaturation_Amount = 1.0f;

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

    Particle_System.reset(new ParticleSystem());
	
	/**/
	oFPS_Text = new Text();
	oFPS_Text->Set_Render_Layer(RENDER_LAYER_SCREEN);
	oFPS_Text->Set_Text("FPS: 0");
	oFPS_Text->Set_Font(Game::Instance()->oMedia->mFonts["test"]);
	oFPS_Text->Set_Size(30);
	oFPS_Text->Set_X(0.0f);
	oFPS_Text->Set_Y(0.0f);
	oFPS_Text->Set_Z(-10.0f);
	oFPS_Text->Ready_For_Rendering();

	oRender_Frame_Time_Text = new Text();
	oRender_Frame_Time_Text->Set_Render_Layer(RENDER_LAYER_SCREEN);
	oRender_Frame_Time_Text->Set_Text("Time to render: 0ms");
	oRender_Frame_Time_Text->Set_Font(Game::Instance()->oMedia->mFonts["test"]);
	oRender_Frame_Time_Text->Set_Size(30);
	oRender_Frame_Time_Text->Set_X(0.0f);
	oRender_Frame_Time_Text->Set_Y(30.0f);
	oRender_Frame_Time_Text->Set_Z(-10.0f);
	oRender_Frame_Time_Text->Ready_For_Rendering();


	oObj_Count_Text = new Text();
	oObj_Count_Text->Set_Render_Layer(RENDER_LAYER_SCREEN);
	oObj_Count_Text->Set_Text("Num objs: 0");
	oObj_Count_Text->Set_Font(Game::Instance()->oMedia->mFonts["test"]);
	oObj_Count_Text->Set_Size(20);
	oObj_Count_Text->Set_X(0.0f);
	oObj_Count_Text->Set_Y(60.0f);
	oObj_Count_Text->Set_Z(-10.0f);
	oObj_Count_Text->Ready_For_Rendering();

	Text* blah = new Text();
	blah->Set_Render_Layer(RENDER_LAYER_WORLD);
	blah->Set_Text("This is a Text object that exists in the world.\nIt will respect camera position and zoom like\nany other object!");
	blah->Set_Font(Game::Instance()->oMedia->mFonts["boris"]);
	blah->Set_Size(40);
	blah->Set_X(-600.0f);
	blah->Set_Y(-50.0f);
	blah->Set_Z(10.0f);
	blah->Set_Line_Height_Padding(10.0f);
	blah->Ready_For_Rendering();

    bRunning = true;
	sf::Clock* Frame_Render_Time = new sf::Clock;

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

			oFPS_Text->Set_Text("fps: " + boost::lexical_cast<std::string>(iCurrent_FPS));
			oFPS_Text->Ready_For_Rendering();

			oRender_Frame_Time_Text->Set_Text("Time to render: " + boost::lexical_cast<std::string>(iRender_Frame_Time) + "ms");
			oRender_Frame_Time_Text->Ready_For_Rendering();

			oObj_Count_Text->Set_Text("Obj count: " + boost::lexical_cast<std::string>(Registered_Entities.size()));
			oObj_Count_Text->Ready_For_Rendering();

        }		

		Frame_Render_Time->restart();
        Render();
		iRender_Frame_Time = Frame_Render_Time->getElapsedTime().asMilliseconds();

		Cleanup_Deleted_Entities();

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

	// Create OpenGL Viewport
    glViewport(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
	glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);	
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);

	// Create render object
	oRenderer = new Renderer();

	return true;

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

	// Handle keyboard and mouse wheel events
    aKeys_Released.clear();
	Mouse_Wheel_Delta = 0;

    sf::Event Event;
    while(oWindow->pollEvent(Event))
    {

        if(Event.type == sf::Event::KeyReleased)
            aKeys_Released.push_back(Event.key.code);
        if(Event.type == sf::Event::MouseWheelMoved)		
			Mouse_Wheel_Delta += Event.mouseWheel.delta;

    }

	// Handle mouse position
	sf::Vector2i pos = sf::Mouse::getPosition(*oWindow);
	aMouse_Pos[0] = ((float)pos.x / (float)DEFAULT_SCREEN_WIDTH) * (float)OPTIMAL_SCREEN_WIDTH;
	aMouse_Pos[1] = ((float)pos.y / (float)DEFAULT_SCREEN_HEIGHT) * (float)OPTIMAL_SCREEN_HEIGHT;
	
	// Lock to window boundaries
	if(aMouse_Pos[0] < 0.0f)
		aMouse_Pos[0] = 0.0f;
	if(aMouse_Pos[0] > OPTIMAL_SCREEN_WIDTH)
		aMouse_Pos[0] = OPTIMAL_SCREEN_WIDTH;
	if(aMouse_Pos[1] < 0.0f)
		aMouse_Pos[1] = 0.0f;
	if(aMouse_Pos[1] > OPTIMAL_SCREEN_HEIGHT)
		aMouse_Pos[1] = OPTIMAL_SCREEN_HEIGHT;

	// Get mouse button states
	aMouse_Buttons[MOUSE_LEFT] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	aMouse_Buttons[MOUSE_RIGHT] = sf::Mouse::isButtonPressed(sf::Mouse::Right);
	aMouse_Buttons[MOUSE_MIDDLE] = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
	aMouse_Buttons[MOUSE_EXTRA_1] = sf::Mouse::isButtonPressed(sf::Mouse::XButton1);
	aMouse_Buttons[MOUSE_EXTRA_2] = sf::Mouse::isButtonPressed(sf::Mouse::XButton2);

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
	// TODO get rid
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

	// Tell the renderer to do it's thing
	oRenderer->Render();

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
	delete(oRenderer);

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


/**
 * Converts world coordinates into screen coordinates between 0 and 1.
 */
std::vector<float> Game::World_To_Normalised_Screen_Coordinates(float x, float y)
{

	// Because the game shows more on wider screen sizes regardless of the 
	// relationship between it and the optimal screen size, the aspect ratio we
	// take into account is that of the actual screen size.
	float aspect_ratio = (float)DEFAULT_SCREEN_WIDTH / (float)DEFAULT_SCREEN_HEIGHT;
	float aov = tan(Deg_To_Rad(ANGLE_OF_VIEW));

	std::vector<float> return_vector;
	return_vector.push_back((x - aCamera_Position[0]) / (aCamera_Position[2] * aov));
	return_vector.push_back(((y - aCamera_Position[1]) * aspect_ratio) / (aCamera_Position[2] * aov));
	return_vector[0] = (return_vector[0] + 1.0f) / 2.0f;
	return_vector[1] = (return_vector[1] + 1.0f) / 2.0f;

	return return_vector;

}


/**
 * Converts world coordinates into optimal screen coordinates,
 * irregardless of the actual screen resolution.
 */
std::vector<float> Game::World_To_Screen(float x, float y)
{
	
	std::vector<float> return_vector = World_To_Normalised_Screen_Coordinates(x, y);
	return_vector[0] *= OPTIMAL_SCREEN_WIDTH;
	return_vector[1] *= OPTIMAL_SCREEN_HEIGHT;

	return return_vector;

}


/**
 * Converts world coordinetes to screen coordinates, as if the 
 * screen was the actual resolution and not the optimal resolution.
 */
std::vector<float> Game::World_To_Real_Screen(float x, float y)
{

	std::vector<float> return_vector = World_To_Normalised_Screen_Coordinates(x, y);
	return_vector[0] *= DEFAULT_SCREEN_WIDTH;
	return_vector[1] *= DEFAULT_SCREEN_HEIGHT;

	return return_vector;

}


/**
 * The inverse of World_To_Normalised_Screen_Coordinates()
 */
std::vector<float> Game::Normalised_Screen_To_World_Coordinates(float x, float y)
{

	float aov = tan(Deg_To_Rad(ANGLE_OF_VIEW));
	float aspect_ratio = (float)DEFAULT_SCREEN_WIDTH / (float)DEFAULT_SCREEN_HEIGHT;

	std::vector<float> return_vector;
	x = (x * 2.0f) - 1.0f;
	return_vector.push_back((x * (aCamera_Position[2] * aov)) + aCamera_Position[0]);
	y = (y * 2.0f) - 1.0f;
	return_vector.push_back(((y / aspect_ratio) * (aCamera_Position[2] * aov)) + aCamera_Position[1]);

	return return_vector;

}


/**
 * Converts screen coordinates to world coordinates as if the screen
 * coordinates supplied were from an optimal screen resolution.
 */
std::vector<float> Game::Screen_To_World(float x, float y)
{

	std::vector<float> return_vector = Normalised_Screen_To_World_Coordinates(
		x / OPTIMAL_SCREEN_WIDTH,
		y / OPTIMAL_SCREEN_HEIGHT
		);
	return return_vector;

}


/**
 * Converts screen coordinates to world coordinates as if the screen
 * coordinates supplied were window coordinates.
 */
std::vector<float> Game::Real_Screen_To_World(float x, float y)
{

	std::vector<float> return_vector = Normalised_Screen_To_World_Coordinates(
		x / DEFAULT_SCREEN_WIDTH,
		y / DEFAULT_SCREEN_HEIGHT
		);
	return return_vector;

}


/**
 * Converts a pixel distance within the world to a screen one
 * in an optimal screen resolution.
 */
float Game::World_Length_To_Screen(float len)
{

	float aov = tan(Deg_To_Rad(ANGLE_OF_VIEW));
	return (len / (aCamera_Position[2] * aov)) * ((float)OPTIMAL_SCREEN_WIDTH / 2);

}


/**
 * Converts a pixel distance within the world to a screen one
 * in the current screen resolution.
 */
float Game::World_Length_To_Real_Screen(float len)
{

	float aov = tan(Deg_To_Rad(ANGLE_OF_VIEW));
	return (len / (aCamera_Position[2] * aov)) * ((float)DEFAULT_SCREEN_WIDTH / 2);

}


/**
 * Converts a pixel distance on the screen to one in world scale
 * as if the screen was an optimal resolution.
 */
float Game::Screen_Length_To_World(float len)
{

	float aov = tan(Deg_To_Rad(ANGLE_OF_VIEW));
	return (len / ((float)OPTIMAL_SCREEN_WIDTH / 2) * (aCamera_Position[2] * aov));

}


/**
 * Converts a pixel distance on the screen to one in world scale
 * as if the screen the actual resolution.
 */
float Game::Real_Screen_Length_To_World(float len)
{

	float aov = tan(Deg_To_Rad(ANGLE_OF_VIEW));
	return (len / ((float)DEFAULT_SCREEN_WIDTH / 2) * (aCamera_Position[2] * aov));

}


/**
 * Takes two vectors containing two elements representing two points. Does a simple
 * formula to return the distance between those points.
 */
float Game::Get_Distance_Between_Points(std::vector<float> point1, std::vector<float> point2)
{

	return std::sqrt((std::pow((point2[1] - point1[1]), 2) + std::pow((point2[0] - point1[0]), 2)));

}


/**
 * Takes two vectors each containing the two coordinates between two points and
 * returns the angle between those in degrees.
 */
float Game::Angle_Between_Points(std::vector<float> point1, std::vector<float> point2)
{

	return Rad_To_Deg(std::atan2(point2[1] - point1[1], point2[0] - point1[0]));

}


/**
 *Returns an equivalent angle value between 0 and 360
 */
float Game::Normalise_Angle(float angle)
{

	return std::fmod(angle, 360.0f);

}   


/**
 * Returns the angle to turn by to get from start_angle to end_angle.
 * The sign of the result indicates the direction in which to turn.
 */
float Game::Angle_Difference(float start_angle, float end_angle)
{

	start_angle = Normalise_Angle(start_angle);
	end_angle = Normalise_Angle(end_angle);
        
	float difference = end_angle - start_angle;
    if(difference > 180.0f)
		difference -= 360.0f;
	if(difference < -180.0f)
		difference += 360.0f;
            
	return difference;

}
    

/**
 * Returns an angle which has been moved from curr_angle closer to
 * targ_angle by increment. increment should always be positive, as
 * angle will be rotated in the direction resulting in the shortest
 * distance to the target angle.
 */
float Game::Near_Angle(float curr_angle, float targ_angle, float increment)
{

	// calculate difference
	float difference = Angle_Difference(curr_angle, targ_angle);

	// Normalise angles
	curr_angle = Normalise_Angle(curr_angle);
	targ_angle = Normalise_Angle(targ_angle);
            
	// do increment
	if(std::fabs(difference) < increment)
		return targ_angle;
	else
		return curr_angle + (increment * (difference / std::fabs(difference)));

}


/**
 * Does an in-place point rotation.
 */
std::vector<float> Game::Rotate_Point(float x, float y, float rotation)
{

	rotation = Deg_To_Rad(rotation);
	std::vector<float> return_vec;
	return_vec.push_back(std::cos(rotation) * x - std::sin(rotation) * y);
	return_vec.push_back(std::sin(rotation) * x + std::cos(rotation) * y);

    return return_vec;

}


/**
 * Does a point rotation around another reference point.
 */
std::vector<float> Game::Rotate_Point_About_Point(float x, float y, float rotation, float rotate_about_x, float rotate_about_y)
{

    std::vector<float> return_vec = Rotate_Point(x - rotate_about_x, y - rotate_about_y, rotation);
	return_vec[0] = return_vec[0] + rotate_about_x;
	return_vec[1] = return_vec[1] + rotate_about_y;

    return return_vec;

}


// Static initialisers
Game* Game::oInst = NULL;
