/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************/

/**
 * Stress effect object source file.
 */


// Includes
#include "../core/consts.h"
#include "../core/Game.h"
#include "../core/Media.h"
#include "StressEffect.h"


/**
 * Constructor
 */
StressEffect::StressEffect(float x, float y, int rot) : Entity()
{

	Set_Render_Layer(RENDER_LAYER_WORLD);
    Set_X(x);
    Set_Y(y);
	Set_Z(0.0f);
    Set_Image(oGame->oMedia->mImages["stresseffect"]);

    iRotation_To = rot;

}


/**
 * 
 */
void StressEffect::Kill()
{

	Entity::Kill();

}


/**
 * 
 */
void StressEffect::Logic()
{

    Advance_Towards(3.0f, iRotation_To);

    if(Get_X() < -1680.0f || Get_X() > 1680.0f || Get_Y() < -1050.0f || Get_Y() > 1050.0f)
        Kill();

}
