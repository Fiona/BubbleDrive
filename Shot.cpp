/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

/**
 * Shot object source file.
 */


// Includes
#include "Game.h"
#include "Shot.h"


/**
 * Constructor
 */
Shot::Shot(float x, float y, int rot) : Entity()
{

    Set_Image(oGame->oMedia->mImages["shot"]);
    Set_X(x);
    Set_Y(y);
    iRotation_To = rot;

}


/**
 * 
 */
void Shot::Logic()
{

    Advance_Towards(3.0f, iRotation_To);

    if(Get_X() < 50.0f || Get_X() > 974.0f || Get_Y() < 0.0f || Get_Y() > 800.0f)
        Kill();

}
