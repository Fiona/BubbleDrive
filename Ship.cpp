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
#include "Game.h"
#include "Ship.h"
#include "Shot.h"


/**
 * Constructor
 */
Ship::Ship() : Entity()
{

    Set_Image(oGame->oMedia->mImages["ship"]);
    Set_X(300.0f);
    Set_Y(300.0f);
    iImage_Frame = 1;

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

    create_vorticies(341.0f, 384.0f, 1);
    create_vorticies(682.0f, 384.0f, 1);

}


void Ship::create_vorticies(float x, float y, int type)
{

    int range = 0;
    int amount = 3;

    if(oGame->Keyboard_Key_Down(sf::Keyboard::Space))
    {
        range = 10;
        amount = 10;
    }

    for(int c = 0; c <= range; c++)
    {
        if(type == 1)
            current_rotation_2 -= amount;
        else
            current_rotation += amount;

        if(type == 1)
        {
            if(current_rotation_2 < -360)
                current_rotation_2 = 0;
        }
        else
        {
            if(current_rotation > 360)
                current_rotation = 0;
        }

        new Shot(x, y, (type == 1 ? current_rotation_2 : current_rotation));        
    }

}
