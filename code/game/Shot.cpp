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
#include "../core/consts.h"
#include "../core/Game.h"
#include "../core/Media.h"
#include "../utils/Vector2D.h"
#include "Shot.h"
#include "Ship.h"


/**
 * Constructor
 */
Shot::Shot(float x, float y, int rot, Ship* parent) : Entity()
{
    
    // Move it
    Parent = parent;
    Velocity.reset(new Vector2D());
    *Velocity += *parent->Velocity;
    *Velocity += Vector2D(oGame->Deg_To_Rad(rot), 20.0f, true);

    // Display everything
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

    // Pew pew
	Set_X(Get_X() + Velocity->xCom);
	Set_Y(Get_Y() + Velocity->yCom);

    // Go life die etc
	Life += 1;
	if(Life > 100)
		Set_Alpha(Get_Alpha() - .05f);
    if(Get_Alpha() < 0.0f)
        Kill();

}
