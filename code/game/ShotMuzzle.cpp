/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************/

/**
 * Shot muzzle object source file.
 */


// Includes
#include "../core/consts.h"
#include "../core/Game.h"
#include "../core/Media.h"
#include "../rendering/Light.h"
#include "ShotMuzzle.h"
#include "Ship.h"

/**
 * Constructor
 */
ShotMuzzle::ShotMuzzle(Ship* parent_ship, int mount_point_num, int rot) : Entity()
{

    Parent = parent_ship;
    Mount_Point_Num = mount_point_num;

	Set_Render_Layer(RENDER_LAYER_WORLD_ABOVE_LIT);
	Set_Z(10.0f);
	Set_Rotation(rot);
    Set_Image(oGame->oMedia->mImages["laser1"]);
	Set_Image_Frame(1);
	Set_Colour(0.6f, 1.0f, 0.6f);
	Life = 0;

    Muzzle_Light = std::make_shared<Light>();
	Muzzle_Light->Set_Size(35);
	Muzzle_Light->Set_Alpha(.4f);

    Update_Pos();

}


/**
 * 
 */
void ShotMuzzle::Logic()
{

    Update_Pos();

	Life += 1;

	if(Life > 4)
    {
		Set_Alpha(Get_Alpha() - .05f);
        Muzzle_Light->Set_Alpha(Muzzle_Light->Get_Alpha() - .02);
    }

    if(Muzzle_Light->Get_Alpha() < 0.0f)
        Kill();

}


void ShotMuzzle::Update_Pos()
{

    auto mount_point_loc = Parent->Get_Mount_Point_Location(Mount_Point_Num);

    Set_X(mount_point_loc[0]);
    Set_Y(mount_point_loc[1]);

	Muzzle_Light->Set_X(Get_X());
	Muzzle_Light->Set_Y(Get_Y());

}
