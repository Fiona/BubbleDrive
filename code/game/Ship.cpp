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
#include "StressEffect.h"
#include "ShockwaveEffect.h"
#include "Shot.h"

boost::random::mt19937 gen;

/**
 * Constructor
 */
Ship::Ship() : Entity()
{

    Velocity = new Vector2D(0.0f, 0.0f);
	Max_Velocity = 3.0f;
	Velocity_Friction = 0.999f;
	Acceleration = 0.010f;
	Max_Acceleration = 0.2f;		
	Turn_Rate = 6.0f;

	Thrusters = new std::map<char, float>;
	(*Thrusters)['f'] = 0.0f;
	(*Thrusters)['b'] = 0.0f;
	(*Thrusters)['l'] = 0.0f;
	(*Thrusters)['r'] = 0.0f;
	
	Reload_Time = 60;

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

	// Basic movement input
    if(oGame->Keyboard_Key_Down(sf::Keyboard::Left) || oGame->Keyboard_Key_Down(sf::Keyboard::A))
		(*Thrusters)['l'] += Acceleration;
    if(oGame->Keyboard_Key_Down(sf::Keyboard::Right) || oGame->Keyboard_Key_Down(sf::Keyboard::E) || oGame->Keyboard_Key_Down(sf::Keyboard::D))
		(*Thrusters)['r'] += Acceleration;
    if(oGame->Keyboard_Key_Down(sf::Keyboard::Up) || oGame->Keyboard_Key_Down(sf::Keyboard::Comma) || oGame->Keyboard_Key_Down(sf::Keyboard::W))
		(*Thrusters)['f'] += Acceleration;
    if(oGame->Keyboard_Key_Down(sf::Keyboard::Down) || oGame->Keyboard_Key_Down(sf::Keyboard::O) | oGame->Keyboard_Key_Down(sf::Keyboard::S))
		(*Thrusters)['b'] += Acceleration;

	// Cap acceleration on thrusters
	for(std::map<char, float>::iterator it = Thrusters->begin(); it != Thrusters->end(); ++it)
	{

		if(it->second > Max_Acceleration)
			(*Thrusters)[it->first] = Max_Acceleration;
		if(it->second < -Max_Acceleration)
			(*Thrusters)[it->first] = -Max_Acceleration;

		(*Thrusters)[it->first] *= 0.95;

	}

	// Increase velocity via thrusters
	*Velocity += Vector2D(oGame->Deg_To_Rad(Get_Rotation()), (*Thrusters)['f'], true);
	*Velocity -= Vector2D(oGame->Deg_To_Rad(Get_Rotation()), (*Thrusters)['b'], true);
	*Velocity += Vector2D(oGame->Deg_To_Rad(Get_Rotation() - 90.0f), (*Thrusters)['l'], true);
	*Velocity += Vector2D(oGame->Deg_To_Rad(Get_Rotation() + 90.0f), (*Thrusters)['r'], true);

	// Update position vector
	*Velocity *= Velocity_Friction;
	Set_X(Get_X() + Velocity->xCom);
	Set_Y(Get_Y() + Velocity->yCom);

	// Cap velocity
	if(Velocity->xCom < -Max_Velocity)
		Velocity->xCom = -Max_Velocity;
	if(Velocity->xCom > Max_Velocity)
		Velocity->xCom = Max_Velocity;
	if(Velocity->yCom < -Max_Velocity)
		Velocity->yCom = -Max_Velocity;
	if(Velocity->yCom > Max_Velocity)
		Velocity->yCom = Max_Velocity;

	// Rotate mouse towards the cursor
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
					Turn_Rate
				)	
			);

	}

	// Other inputs
	if(oGame->Mouse_Wheel_Delta != 0)
		oGame->aCamera_Position[2] -= (oGame->Mouse_Wheel_Delta * 60);

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

	if(Reload_Time < 20)
		Reload_Time += 1;

	if(Reload_Time >= 20 && oGame->aMouse_Buttons[MOUSE_LEFT])
	{

		std::vector<float> mount_point; mount_point.push_back(6); mount_point.push_back(9);
		new Shot(Get_X(), Get_Y(), Get_Rotation(), mount_point);

		mount_point[1] = -9;
		new Shot(Get_X(), Get_Y(), Get_Rotation(), mount_point);

		Reload_Time = 0;

	}

	if(Reload_Time >= 10 && oGame->aMouse_Buttons[MOUSE_RIGHT])
	{
        new ShockwaveEffect(mouse_position_in_world[0], mouse_position_in_world[1], 0.01f, 5);
		Reload_Time = 0;
	}

	if(oGame->Keyboard_Key_Down(sf::Keyboard::Space))
	{

		for(int i = 0; i < 5; i++)
		{
			boost::random::uniform_int_distribution<> dist(-25, 25);
		    int x = dist(gen);
			int y = dist(gen);

			boost::random::uniform_int_distribution<> dist2(0, 360);
			int angle = dist2(gen);

			new StressEffect((float)x, (float)y, angle);

		}

	}

	// Update camera position
	std::vector<float> centre = Get_Hotspot_Pos(HOTSPOT_CENTRE);
	oGame->aCamera_Position[0] = centre[0];
	oGame->aCamera_Position[1] = centre[1];

}

