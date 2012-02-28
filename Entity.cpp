/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

/**
   Generic Entity object source file.
 */


// Includes
#include "Entity.h"
#include "Game.h"
#include <iostream>


/**
 * Constructor
 */
Entity::Entity()
{

    fX = 0.0f;
    fY = 0.0f;
    iZ = 0;
    oImage = NULL;
    oGame = Game::Instance();
    oGame->Register_Entity(this);

}


/**
 * Deconstructor
 */
Entity::~Entity()
{
}


/**
 * This method is designed to be overridden by sub classes.
 * It's called for every registered Entity on every game tick.
 * The game attempts to tick 60 times a second.
 */
void Entity::Logic()
{

}


/**
 * This is called for every Entity as much as possible to while ensuring
 * a steady framerate.
 * By default it will draw at the right position, rotation, scale etc.
 * It's designed to be overridden if necessary to add some custom drawing.
 */
void Entity::Draw()
{
    
    glPushMatrix();

    // Get drawing coords
    float X = Get_X();
    float Y = Get_Y();

    // move to position
    glTranslatef(X, Y, 0.0f);

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, Get_Image()->iTexture_Num);
    glVertexPointer(3, GL_FLOAT, 0, Get_Image()->aVertex_List);

    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

    // draw the triangle strip
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glPopMatrix();

}


/**
 * Basic Getters/Setters
 * Can all be overriden.
 */
void Entity::Set_X(float X)
{
    fX = X;
}

float Entity::Get_X()
{
    return fX;
}

void Entity::Set_Y(float Y)
{
    fY = Y;
}

float Entity::Get_Y()
{
    return fY;
}

void Entity::Set_Z(int Z)
{
    iZ = Z;
}

int Entity::Get_Z()
{
    return iZ;
}

void Entity::Set_Image(Image* Image)
{
    oImage = Image;
}

Image* Entity::Get_Image()
{
    return oImage;
}
