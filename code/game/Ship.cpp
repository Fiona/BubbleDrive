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
    Set_Image_Frame(1);
    Set_Image(oGame->oMedia->mImages["ship"]);

    current_rotation = 0;
    current_rotation_2 = 0;

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

    if(oGame->Keyboard_Key_Down(sf::Keyboard::Num1))
        Set_Rotation(Get_Rotation() - 2.0f);
    if(oGame->Keyboard_Key_Down(sf::Keyboard::Num2))
        Set_Rotation(Get_Rotation() + 2.0f);

    if(oGame->Keyboard_Key_Down(sf::Keyboard::Num3))
        Set_Scale(Get_Scale() - .05f);
    if(oGame->Keyboard_Key_Down(sf::Keyboard::Num4))
        Set_Scale(Get_Scale() + .05f);

    if(oGame->Keyboard_Key_Down(sf::Keyboard::Num5))
		oGame->fScreen_Blur_Amount = 1.0f;

    if(oGame->fScreen_Blur_Amount > 0.0f)
		oGame->fScreen_Blur_Amount -= 0.1f;
	else
		oGame->fScreen_Blur_Amount = 0.0f;

	current_rotation += 90;
	if(current_rotation > 360)
		current_rotation = 0;

	for(int i = 0; i < 1; i++)
	{
		boost::random::uniform_int_distribution<> dist(0, 500);
	    int x = dist(gen);
		int y = dist(gen);

		boost::random::uniform_int_distribution<> dist2(0, 90);
	    int angle = dist(gen);

		new Shot((float)x, (float)y, angle);

	}
    //create_vorticies(200.0f, 200.0f, 1);
    //create_vorticies(300.0f, 300.0f, 1);
    //create_vorticies(600.0f, 450.0f, 1);
    //create_vorticies(840.0f, 525.0f, 1);

	std::vector<float> centre = Get_Hotspot_Pos(HOTSPOT_CENTRE);
	oGame->aCamera_Position[0] = centre[0];
	oGame->aCamera_Position[1] = centre[1];

}


void Ship::create_vorticies(float x, float y, int type)
{

    int range = 0;
    int amount = 45;

    //if(oGame->Keyboard_Key_Down(sf::Keyboard::Space))
    //{
		range = 0;
        amount = 32;
    //}

    for(int c = 0; c <= range; c++)
    {
	    current_rotation += amount;

        if(current_rotation > 360)
	        current_rotation = 0;

        new Shot(x, y, current_rotation);
    }

}
