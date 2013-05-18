/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

/**
 * Ship object source file.
 */


// Includes
#include <iostream>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "../core/Game.h"
#include "Ship.h"
#include "Shot.h"

boost::random::mt19937 gen;

/**
 * Constructor
 */
Ship::Ship() : Entity()
{

	Set_Render_Layer(RENDER_LAYER_WORLD_LIT);
    Set_X(0.0f);
    Set_Y(0.0f);
	Set_Z(.5f);
    Set_Image(oGame->oMedia->mImages["ship"]);

}


/**
 * 
 */
void Ship::Logic()
{

    if(oGame->Keyboard_Key_Down(sf::Keyboard::Left))
        Set_X(Get_X() - 10.0f);
    if(oGame->Keyboard_Key_Down(sf::Keyboard::Right))
        Set_X(Get_X() + 10.0f);
    if(oGame->Keyboard_Key_Down(sf::Keyboard::Up))
        Set_Y(Get_Y() - 10.0f);
    if(oGame->Keyboard_Key_Down(sf::Keyboard::Down))
        Set_Y(Get_Y() + 10.0f);

	std::vector<float> mouse_position_in_world = oGame->Screen_To_World(oGame->aMouse_Pos[0], oGame->aMouse_Pos[1]);	
	std::vector<float> ship_pos; ship_pos.push_back(Get_X()); ship_pos.push_back(Get_Y());
	float distance_between_mouse_and_ship = oGame->Get_Distance_Between_Points(mouse_position_in_world, ship_pos);

	if(distance_between_mouse_and_ship > 32.0f)
	{

		float rotation_towards_mouse = oGame->Angle_Between_Points(ship_pos, mouse_position_in_world);
		Set_Rotation(
				oGame->Near_Angle(
					Get_Rotation(),
					rotation_towards_mouse,
					5.0f
				)	
			);

	}

    if(oGame->Keyboard_Key_Down(sf::Keyboard::Num1))
		oGame->fSaturation_Amount -= .05f;
	else
		oGame->fSaturation_Amount += .05f;

    if(oGame->fSaturation_Amount > 1.0f)
		oGame->fSaturation_Amount = 1.0f;
    if(oGame->fSaturation_Amount < 0.0f)
		oGame->fSaturation_Amount = 0.0f;

    if(oGame->Keyboard_Key_Down(sf::Keyboard::Num2))
		oGame->fScreen_Blur_Amount = 1.0f;

    if(oGame->fScreen_Blur_Amount > 0.0f)
		oGame->fScreen_Blur_Amount -= 0.1f;
	else
		oGame->fScreen_Blur_Amount = 0.0f;

	if(oGame->Keyboard_Key_Down(sf::Keyboard::Space))
	{

		for(int i = 0; i < 5; i++)
		{
			boost::random::uniform_int_distribution<> dist(-25, 25);
		    int x = dist(gen);
			int y = dist(gen);

			boost::random::uniform_int_distribution<> dist2(0, 360);
			int angle = dist2(gen);

			new Shot((float)x, (float)y, angle);

		}

	}

	std::vector<float> centre = Get_Hotspot_Pos(HOTSPOT_CENTRE);
	oGame->aCamera_Position[0] = centre[0];
	oGame->aCamera_Position[1] = centre[1];

}

