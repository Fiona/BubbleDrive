/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Nov 2012 by Fiona ****
 ***************************************/

/**
 * Test object source file.
 */


// Includes
#include <iostream>
#include <math.h>
#include "Test.h"
#include "../core/consts.h"
#include "../core/Media.h"
#include "../core/Game.h"
#include "../rendering/Light.h"

/**
 * Constructor
 */
Test::Test(int type) : Entity()
{

	Set_Render_Layer(RENDER_LAYER_WORLD_LIT);
	Set_Z(.5f);
    Set_Image(oGame->oMedia->mImages["stresseffect"]);
	Set_Scale(.1);

    oMy_Light = std::make_shared<Light>();
	oMy_Light->Set_Size(300);
	oMy_Light->Set_Alpha(1.2f);

	iType = type;

	if(iType == 1)
	{
		oMy_Light->Set_Colour(1.0, 0.3, 0.3);
		life = 0;
	}
	if(iType == 2)
	{
		oMy_Light->Set_Colour(0.3, 1.0, 0.3);
		life = 90;
	}
	if(iType == 3)
	{
		oMy_Light->Set_Colour(1.0, 0.0, 1.0);
		life = 150;
	}

}


/**
 * 
 */
void Test::Logic()
{
	life += 1;
	float life_scale = (0.01*2*3.1415926f)/2;

	Set_X(std::sin((float)life * life_scale)*250.0);
	Set_Y(std::cos((float)life * life_scale)*250.0);

	oMy_Light->Set_X(Get_X());
	oMy_Light->Set_Y(Get_Y());

}
