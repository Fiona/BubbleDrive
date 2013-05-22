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
#include "../core/Game.h"
#include "Shot.h"


/**
 * Constructor
 */
Shot::Shot(float x, float y, int rot, std::vector<float> mount_point) : Entity()
{

	Set_Render_Layer(RENDER_LAYER_WORLD);
    Set_X(x);
    Set_Y(y);
	Set_Z(0.0f);
	Set_Rotation(rot);
    Set_Image(oGame->oMedia->mImages["laser1"]);
	Set_Image_Frame(0);
	Set_Colour(.5f, 0.0f, .5f);
	Life = 0;

	// Adjust pos based on mount point
	std::vector<float> adjust_pos = oGame->Rotate_Point(mount_point[0], mount_point[1], rot);
	Set_X(Get_X() + adjust_pos[0]);
	Set_Y(Get_Y() + adjust_pos[1]);

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
