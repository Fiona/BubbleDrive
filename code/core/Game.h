/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _GAME_H_
#define _GAME_H_

/**
 * Game object header
 */

// Windows specific
#if _WIN32
#include <windows.h>
#endif

// Includes
#include <iostream>
#include <cstddef>
#include <vector>
#include <map>
#include <boost/lexical_cast.hpp>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SFML/Window.hpp>
#include "Media.h"
#include "State.h"
#include "Entity.h"
#include "../text/FontManager.h"
#include "../text/Text.h"
#include "../rendering/Shader.h"
#include "../rendering/Renderer.h"

// Forward declaration
class Entity;
class Text;
class Renderer;


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
    std::vector<sf::Keyboard::Key> aKeys_Released;
    std::vector<Entity*> Registered_Entities;
    std::vector<Entity*> Entities_To_Delete;
	Text* oFPS_Text;
	Text* oObj_Count_Text;

    bool Initialise_Graphics();
    void Load_Media();
    void Tick();
    void Update_Events();
    void Render();
	void Cleanup_Deleted_Entities();
    void Shutdown();

public:
    sf::Window* oWindow;
    bool bRunning;
    int iCurrent_FPS;
	FontManager* oFont_Manager;
    Media* oMedia;
    State* oState;
	Renderer* oRenderer;
    GLuint iCurrent_Bound_Texture;
    std::vector<float> oDefault_Texture_Coords;
	float aCamera_Position[4];
	std::vector<float> aMouse_Pos;
	std::vector<bool> aMouse_Buttons;

	float fScreen_Blur_Amount;

    Game();
    static Game* Instance();
    int Start();
    void Register_Entity(Entity* Entity_To_Register);
    void Unregister_Entity(Entity* Entity_To_Unregister);
    bool Keyboard_Key_Down(sf::Keyboard::Key k);
    bool Keyboard_Key_Released(sf::Keyboard::Key k);
    float Deg_To_Rad(float degrees);
    float Rad_To_Deg(float radians);
	std::string Load_From_File(std::string file_name);
	std::vector<float> World_To_Normalised_Screen_Coordinates(float x, float y);
	std::vector<float> World_To_Screen(float x, float y);
	std::vector<float> World_To_Real_Screen(float x, float y);
	std::vector<float> Normalised_Screen_To_World_Coordinates(float x, float y);
	std::vector<float> Screen_To_World(float x, float y);
	std::vector<float> Real_Screen_To_World(float x, float y);
	float World_Length_To_Screen(float len);
	float World_Length_To_Real_Screen(float len);
	float Screen_Length_To_World(float len);
	float Real_Screen_Length_To_World(float len);
	float Get_Distance_Between_Points(std::vector<float> point1, std::vector<float> point2);
	float Angle_Between_Points(std::vector<float> point1, std::vector<float> point2);
	float Normalise_Angle(float angle);
	float Angle_Difference(float start_angle, float end_angle);
	float Near_Angle(float curr_angle, float targ_angle, float increment);

};


#endif
