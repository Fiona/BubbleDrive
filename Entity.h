/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#ifndef _ENTITY_H_
#define _ENTITY_H_

/**
 * Generic game Entity object header
 */


// Includes
#include <vector>
#include "Game.h"


// Forward declaration
class Game;


/**
 * All visible objects in the game inherit from the generic
 * Entity class. It handles basic drawing, screen position,
 * rotation etc.
 * When subclassing, the Entity constructor should always be
 * called to ensure that it gets added to the global entity list.
 */
class Entity
{

private:
    float fX;
    float fY;
    int iZ;
    Image* oImage;

public:
    Game* oGame;
    int iImage_Frame;
    float fAlpha;
    std::vector<float> aColour;

    Entity();
    ~Entity();
    virtual void Logic();
    virtual void Draw();
    virtual void Kill();
    virtual void Set_X(float X);
    virtual float Get_X();
    virtual void Set_Y(float Y);
    virtual float Get_Y();
    virtual void Set_Z(int Z);
    virtual int Get_Z();
    virtual void Set_Image(Image* Image);
    virtual Image* Get_Image();
    virtual void Set_Colour(float r, float g, float b);

    void Advance_Towards(float distance, int rot);

};

#endif
