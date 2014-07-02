/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************/

/**
 * Shot object source file.
 */


// Includes
#include "../core/Game.h"
#include "Shot.h"


/**
 * Constructor
 */
Shot::Shot(float x, float y, int rot) : Entity()
{

	Set_Render_Layer(RENDER_LAYER_WORLD);
    Set_X(x);
    Set_Y(y);
	Set_Z(0.0f);
	Set_Rotation(rot);
    Set_Image(oGame->oMedia->mImages["laser1"]);
	Set_Image_Frame(0);
	Set_Colour(0.6f, 1.0f, 0.6f);
	Life = 0;

}


/**
 * 
 */
void Shot::Logic()
{

    Advance_Towards(12.0f, Get_Rotation());
	Life += 1;

	if(Life > 100)
		Set_Alpha(Get_Alpha() - .05f);

    if(Get_Alpha() < 0.0f)
        Kill();

}
