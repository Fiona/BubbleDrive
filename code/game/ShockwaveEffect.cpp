/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************/

/**
 * Shockwave effect object source file.
 */


// Includes
#include "../core/Game.h"
#include "../rendering/Shockwave.h"
#include "ShockwaveEffect.h"


/**
 * Constructor
 */
ShockwaveEffect::ShockwaveEffect(float x, float y, float speed, int size) : Entity()
{

	Set_Render_Layer(RENDER_LAYER_WORLD_LIT);
    Set_Image(oGame->oMedia->mImages["empty"]);

    Set_X(x);
    Set_Y(y);
    Set_Scale(0.0f);

    fSpeed = speed;
    iSize = size;
    iCurrent_Size = 0;

    Update_My_Shockwave();

}


/**
 *
 */
void ShockwaveEffect::Update_My_Shockwave()
{

	oMy_Shockwave.Set_X(Get_X());
	oMy_Shockwave.Set_Y(Get_Y());
	oMy_Shockwave.Set_Size(iCurrent_Size);
	oMy_Shockwave.Set_Alpha(Get_Alpha());

}


/**
 * 
 */
void ShockwaveEffect::Logic()
{

    iCurrent_Size += iSize;

    Set_Alpha(Get_Alpha() - fSpeed);
    Set_Scale((float)iCurrent_Size / (float)Get_Image()->iWidth);

    Update_My_Shockwave();

    if(Get_Alpha() <= 0.0f)
    {
        Kill();
    }

}
