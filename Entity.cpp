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
#include <math.h>


/**
 * Constructor
 */
Entity::Entity()
{

    fX = 0.0f;
    fY = 0.0f;
    iZ = 0;
    iImage_Frame = 1;
    fAlpha = 1.0f;
    aColour.resize(3, 1.0f);
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

    // Bind texture coords if our image is a multi frame one
    if(Get_Image()->iFrame_Count > 1)
        glTexCoordPointer(2, GL_FLOAT, 0, &Get_Image()->aTexture_Coord_Lists[iImage_Frame - 1][0]);

    // Binding texture
    if(oGame->iCurrent_Bound_Texture != Get_Image()->iTexture_Num)
    {
        glBindTexture(GL_TEXTURE_2D, Get_Image()->iTexture_Num);
        glVertexPointer(3, GL_FLOAT, 0, Get_Image()->aVertex_List);
        oGame->iCurrent_Bound_Texture = Get_Image()->iTexture_Num;
    }

    // Colour and transparency
    glColor4f(aColour[0], aColour[1], aColour[2], fAlpha);

    // draw the triangle strip
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Reset the texture coords back to default if necessary
    if(Get_Image()->iFrame_Count > 1)
        glTexCoordPointer(2, GL_FLOAT, 0, &oGame->oDefault_Texture_Coords[0]);

    glPopMatrix();

}


/**
 * This should be used to kill an entity and stop them from being executed and drawn.
 * It's designed to be overridden if necessary to add some additional behaviour upon killing.
 */
void Entity::Kill()
{

   oGame->Unregister_Entity(this);

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

void Entity::Set_Colour(float r, float g, float b)
{
    aColour[0] = r;
    aColour[1] = g;
    aColour[2] = b;
}


/**
 * Will move the X/Y coordinates in a specified directon a specified distance
 */
void Entity::Advance_Towards(float distance, int rot)
{
    Set_X(Get_X() + distance * cos(oGame->Deg_To_Rad((float)rot)));
    Set_Y(Get_Y() + distance * sin(oGame->Deg_To_Rad((float)rot)));
}
