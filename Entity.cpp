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

// Windows specific
#if _WIN32
#include <windows.h>
#endif

// Includes
#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include "Entity.h"
#include "Game.h"



/**
 * Constructor
 */
Entity::Entity()
{

    fX = 0.0f;
    fY = 0.0f;
    iZ = 0.0f;
	fRotation = 0.0f;
	fScale = 1.0f;
    iImage_Frame = 1;
    fAlpha = 1.0f;
	iRender_Mode = RENDER_MODE_SCREEN;
    aColour.resize(3, 1.0f);
    oImage = NULL;
    oGame = Game::Instance();

	oBatches_And_Object_Indicies = new std::vector<int>;
	bIs_Updating_Batches = false;

    oGame->Register_Entity(this);

}


/**
 * Deconstructor
 */
Entity::~Entity()
{
	oBatches_And_Object_Indicies->clear();
	delete(oBatches_And_Object_Indicies);
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
 * This should be used to kill an entity and stop them from being executed and drawn.
 * It's designed to be overridden if necessary to add some additional behaviour upon killing.
 */
void Entity::Kill()
{

	oGame->Unregister_Entity(this);

	if(oBatches_And_Object_Indicies->size() > 0)
		oGame->oBatch_Manager->Request_Removal_Of_Objects(oBatches_And_Object_Indicies, true);

}


/**
 * Called by Batch objects to request the vertex information
 * for the specified object.
 */
void Entity::Get_Object_Index_Data(int object_index, GLfloat* vbo_data, int vbo_offset_start)
{

	// ******
	// Vertex stuff 
	// ******

	// The offset values are for calculating the centre of the triangles
	// which should be 0,0. Rather than 0,0 being the top left.
	float w, h, w_o, h_o;

	if(Get_Image() == NULL)
	{

		w = 0.0f;
		h = 0.0f;
		w_o = 0.0f;
		h_o = 0.0f;

	}
	else
	{

		w = (float)Get_Image()->iWidth;
		h = (float)Get_Image()->iHeight;

		if(Get_Render_Mode() == RENDER_MODE_SCREEN)
		{
			w /= (float)((float)OPTIMAL_SCREEN_WIDTH / (float)DEFAULT_SCREEN_WIDTH);
			h /= (float)((float)OPTIMAL_SCREEN_HEIGHT / (float)DEFAULT_SCREEN_HEIGHT);
		}

		w_o = w/2;
		h_o = h/2;

	}

	// *******
	// Texture coordinate stuff
	// *******
	float sequence_start_pos, texture_x_from, texture_x_to, texture_y_from, texture_y_to;

	texture_x_from = 0.0f;
    texture_x_to = 1.0;
	texture_y_from = 0.0f;
    texture_y_to = 1.0;

	if(Get_Image() && Get_Image()->iFrame_Count > 1)
	{

        sequence_start_pos = 0.01f * (((float)Get_Image()->iWidth / (float)Get_Image()->iRaw_Surface_Width) * 100.0f);
		texture_x_from = sequence_start_pos * Get_Image_Frame();
		texture_x_to = (sequence_start_pos * Get_Image_Frame()) + sequence_start_pos;

	}

	// *******
	// Pos/rot/scale
	// *******
	float x, y, rot, scale;
	x = Get_X();
	y = Get_Y();
	rot = Get_Rotation();
	scale = Get_Scale();

	if(Get_Render_Mode() == RENDER_MODE_SCREEN)
	{
		x /= (float)((float)OPTIMAL_SCREEN_WIDTH / (float)DEFAULT_SCREEN_WIDTH);
		y /= (float)((float)OPTIMAL_SCREEN_HEIGHT / (float)DEFAULT_SCREEN_HEIGHT);
	}

	// *******
	// Data creation
	// *******
	int vjump = vbo_offset_start * NUM_ELEMENTS_PER_VERTEX * NUM_VERTEX_IN_OBJECT;

	//  ---- VERTEX LOCS -------------- COLOURS -------------------------- TEXTURE COORDS -------------- POS/ROTATION/SCALING

	// tri 1 top right	
	vbo_data[vjump] = w - w_o; 	vbo_data[vjump+2] = aColour[0];	vbo_data[vjump+6] = texture_x_to;	vbo_data[vjump+8] = x;
	vbo_data[vjump+1] = -h_o;	vbo_data[vjump+3] = aColour[1];	vbo_data[vjump+7] = texture_y_from;	vbo_data[vjump+9] = y;
								vbo_data[vjump+4] = aColour[2];										vbo_data[vjump+10] = rot;
								vbo_data[vjump+5] = fAlpha;											vbo_data[vjump+11] = scale;

	// tri 1 top left
	vjump += NUM_ELEMENTS_PER_VERTEX;
	vbo_data[vjump] = - w_o; 	vbo_data[vjump+2] = aColour[0];	vbo_data[vjump+6] = texture_x_from;	vbo_data[vjump+8] = x;
	vbo_data[vjump+1] = -h_o;	vbo_data[vjump+3] = aColour[1];	vbo_data[vjump+7] = texture_y_from;	vbo_data[vjump+9] = y;
								vbo_data[vjump+4] = aColour[2];										vbo_data[vjump+10] = rot;
								vbo_data[vjump+5] = fAlpha;											vbo_data[vjump+11] = scale;

	// tri 1 bottom left
	vjump += NUM_ELEMENTS_PER_VERTEX;

	vbo_data[vjump] = -w_o; 	vbo_data[vjump+2] = aColour[0];	vbo_data[vjump+6] = texture_x_from;	vbo_data[vjump+8] = x;
	vbo_data[vjump+1] = h - h_o;vbo_data[vjump+3] = aColour[1];	vbo_data[vjump+7] = texture_y_to;	vbo_data[vjump+9] = y;
								vbo_data[vjump+4] = aColour[2];										vbo_data[vjump+10] = rot;
								vbo_data[vjump+5] = fAlpha;											vbo_data[vjump+11] = scale;

	// tri 2 bottom left
	vjump += NUM_ELEMENTS_PER_VERTEX;
	vbo_data[vjump] = -w_o; 	vbo_data[vjump+2] = aColour[0];	vbo_data[vjump+6] = texture_x_from;	vbo_data[vjump+8] = x;
	vbo_data[vjump+1] = h - h_o;vbo_data[vjump+3] = aColour[1];	vbo_data[vjump+7] = texture_y_to;	vbo_data[vjump+9] = y;
								vbo_data[vjump+4] = aColour[2];										vbo_data[vjump+10] = rot;
								vbo_data[vjump+5] = fAlpha;											vbo_data[vjump+11] = scale;

	// tri 2 bottom right
	vjump += NUM_ELEMENTS_PER_VERTEX;
	vbo_data[vjump] = w - w_o; 	vbo_data[vjump+2] = aColour[0];	vbo_data[vjump+6] = texture_x_to;	vbo_data[vjump+8] = x;
	vbo_data[vjump+1] = h - h_o;vbo_data[vjump+3] = aColour[1];	vbo_data[vjump+7] = texture_y_to;	vbo_data[vjump+9] = y;
								vbo_data[vjump+4] = aColour[2];										vbo_data[vjump+10] = rot;
								vbo_data[vjump+5] = fAlpha;											vbo_data[vjump+11] = scale;

	// tri 2 top right
	vjump += NUM_ELEMENTS_PER_VERTEX;
	vbo_data[vjump] = w - w_o; 	vbo_data[vjump+2] = aColour[0];	vbo_data[vjump+6] = texture_x_to;	vbo_data[vjump+8] = x;
	vbo_data[vjump+1] = -h_o;	vbo_data[vjump+3] = aColour[1];	vbo_data[vjump+7] = texture_y_from;	vbo_data[vjump+9] = y;
								vbo_data[vjump+4] = aColour[2];										vbo_data[vjump+10] = rot;
								vbo_data[vjump+5] = fAlpha;											vbo_data[vjump+11] = scale;

	/**
	// tri 1 top right	
	(*vbo_data)[vjump] = w - w_o; 	(*vbo_data)[vjump+2] = aColour[0];	(*vbo_data)[vjump+6] = texture_x_to;	(*vbo_data)[vjump+8] = x;
	(*vbo_data)[vjump+1] = -h_o;	(*vbo_data)[vjump+3] = aColour[1];	(*vbo_data)[vjump+7] = texture_y_from;	(*vbo_data)[vjump+9] = y;
									(*vbo_data)[vjump+4] = aColour[2];											(*vbo_data)[vjump+10] = rot;
									(*vbo_data)[vjump+5] = fAlpha;												(*vbo_data)[vjump+11] = scale;

	// tri 1 top left
	vjump += NUM_ELEMENTS_PER_VERTEX;
	(*vbo_data)[vjump] = -w_o; 		(*vbo_data)[vjump+2] = aColour[0];	(*vbo_data)[vjump+6] = texture_x_from;	(*vbo_data)[vjump+8] = x;
	(*vbo_data)[vjump+1] = -h_o;	(*vbo_data)[vjump+3] = aColour[1];	(*vbo_data)[vjump+7] = texture_y_from;	(*vbo_data)[vjump+9] = y;
									(*vbo_data)[vjump+4] = aColour[2];											(*vbo_data)[vjump+10] = rot;
									(*vbo_data)[vjump+5] = fAlpha;												(*vbo_data)[vjump+11] = scale;

	// tri 1 bottom left
	vjump += NUM_ELEMENTS_PER_VERTEX;
	(*vbo_data)[vjump] = -w_o; 		(*vbo_data)[vjump+2] = aColour[0];	(*vbo_data)[vjump+6] = texture_x_from;	(*vbo_data)[vjump+8] = x;
	(*vbo_data)[vjump+1] = h - h_o;	(*vbo_data)[vjump+3] = aColour[1];	(*vbo_data)[vjump+7] = texture_y_to;	(*vbo_data)[vjump+9] = y;
									(*vbo_data)[vjump+4] = aColour[2];											(*vbo_data)[vjump+10] = rot;
									(*vbo_data)[vjump+5] = fAlpha;												(*vbo_data)[vjump+11] = scale;

	// tri 2 bottom left
	vjump += NUM_ELEMENTS_PER_VERTEX;
	(*vbo_data)[vjump] = -w_o; 		(*vbo_data)[vjump+2] = aColour[0];	(*vbo_data)[vjump+6] = texture_x_from;	(*vbo_data)[vjump+8] = x;
	(*vbo_data)[vjump+1] = h - h_o;	(*vbo_data)[vjump+3] = aColour[1];	(*vbo_data)[vjump+7] = texture_y_to;	(*vbo_data)[vjump+9] = y;
									(*vbo_data)[vjump+4] = aColour[2];											(*vbo_data)[vjump+10] = rot;
									(*vbo_data)[vjump+5] = fAlpha;												(*vbo_data)[vjump+11] = scale;

	// tri 2 bottom right
	vjump += NUM_ELEMENTS_PER_VERTEX;
	(*vbo_data)[vjump] = w - w_o; 	(*vbo_data)[vjump+2] = aColour[0];	(*vbo_data)[vjump+6] = texture_x_to;	(*vbo_data)[vjump+8] = x;
	(*vbo_data)[vjump+1] = h - h_o;	(*vbo_data)[vjump+3] = aColour[1];	(*vbo_data)[vjump+7] = texture_y_to;	(*vbo_data)[vjump+9] = y;
									(*vbo_data)[vjump+4] = aColour[2];											(*vbo_data)[vjump+10] = rot;
									(*vbo_data)[vjump+5] = fAlpha;												(*vbo_data)[vjump+11] = scale;

	// tri 2 top right
	vjump += NUM_ELEMENTS_PER_VERTEX;
	(*vbo_data)[vjump] = w - w_o; 	(*vbo_data)[vjump+2] = aColour[0];	(*vbo_data)[vjump+6] = texture_x_to;	(*vbo_data)[vjump+8] = x;
	(*vbo_data)[vjump+1] = -h_o;	(*vbo_data)[vjump+3] = aColour[1];	(*vbo_data)[vjump+7] = texture_y_from;	(*vbo_data)[vjump+9] = y;
									(*vbo_data)[vjump+4] = aColour[2];											(*vbo_data)[vjump+10] = rot;
									(*vbo_data)[vjump+5] = fAlpha;												(*vbo_data)[vjump+11] = scale;
	/**/
	bIs_Updating_Batches = false;

}


/**
 * Used by Setters to update the Batch/Object vector
 * when certain properties change.
 * If changing properties that has the potential to shift to different
 * batches (Image/Z/RenderMode) remove_current must be true. Otherwise false.
 */
void Entity::Update_Batches_And_Object_Indicies(bool remove_current)
{

	if(bIs_Updating_Batches && Get_Image() && !remove_current)
		return;

	if(oBatches_And_Object_Indicies->size() == 0)
		oGame->oBatch_Manager->Request_New_Batch_And_Object_Indicies(this, 1, oBatches_And_Object_Indicies);
	else
	{

		if(remove_current)
		{

			oGame->oBatch_Manager->Request_Removal_Of_Objects(oBatches_And_Object_Indicies, false);
			oBatches_And_Object_Indicies->clear();
			oGame->oBatch_Manager->Request_New_Batch_And_Object_Indicies(this, 1, oBatches_And_Object_Indicies);

		}

	}

	oGame->oBatch_Manager->Request_Object_Update_For_Entity(this, oBatches_And_Object_Indicies);

	bIs_Updating_Batches = true;

}


/**
 * Basic Getters/Setters
 * Can all be overriden.
 */
void Entity::Set_X(float X)
{
	if(fX == X)
		return;
    fX = X;
	Update_Batches_And_Object_Indicies(false);
}

float Entity::Get_X()
{
	return fX;
}

void Entity::Set_Y(float Y)
{
	if(fY == Y)
		return;
    fY = Y;
	Update_Batches_And_Object_Indicies(false);
}

float Entity::Get_Y()
{
	return fY;
 }

void Entity::Set_Z(float Z)
{
	if(iZ == Z)
		return;
    iZ = Z;
	Update_Batches_And_Object_Indicies(true);
}

float Entity::Get_Z()
{
    return iZ;
}

void Entity::Set_Rotation(float Rotation)
{
	if(fRotation == Rotation)
		return;
    fRotation = Rotation;
	Update_Batches_And_Object_Indicies(false);
}

float Entity::Get_Rotation()
{
    return fRotation;
}

void Entity::Set_Scale(float Scale)
{
	if(fScale == Scale)
		return;
    fScale = Scale;
	Update_Batches_And_Object_Indicies(false);
}

float Entity::Get_Scale()
{
    return fScale;
}

void Entity::Set_Image(Image* Image)
{
	if(oImage == Image)
		return;
    oImage = Image;
	Update_Batches_And_Object_Indicies(true);
}

Image* Entity::Get_Image()
{
    return oImage;
}

void Entity::Set_Image_Frame(int image_frame)
{
    if(iImage_Frame == image_frame)
		return;
    iImage_Frame = image_frame;
	Update_Batches_And_Object_Indicies(false);
}

int Entity::Get_Image_Frame()
{
    return iImage_Frame;
}

void Entity::Set_Render_Mode(int render_mode)
{
	if(iRender_Mode == render_mode)
		return;
	iRender_Mode = render_mode;
	Update_Batches_And_Object_Indicies(true);
}

int Entity::Get_Render_Mode()
{
	return iRender_Mode;
}

void Entity::Set_Colour(float r, float g, float b)
{
	if(aColour[0] == r && aColour[1] == g && aColour[2] == b)
		return;
    aColour[0] = r;
    aColour[1] = g;
    aColour[2] = b;
	Update_Batches_And_Object_Indicies(false);
}

void Entity::Set_Alpha(float alpha)
{
	if(fAlpha == alpha)
		return;
    fAlpha = alpha;
	Update_Batches_And_Object_Indicies(false);
}

float Entity::Get_Alpha()
{
    return fAlpha;
}


/**
 * Returns a vector of floats pointing to the position of the Entity hotspot
 * in question. HOTSPOT_CENTRE is a special one that points to the centre of the
 * Entity.
 */
std::vector<float> Entity::Get_Hotspot_Pos(int spot)
{

	std::vector<float> hotspot_pos(2, 0.0f);

	if(spot == HOTSPOT_CENTRE)
	{
		hotspot_pos[0] = Get_X();
		hotspot_pos[1] = Get_Y();
	}

	return hotspot_pos;

}

/**
 * Will move the X/Y coordinates in a specified directon a specified distance
 */
void Entity::Advance_Towards(float distance, int rot)
{
    Set_X(Get_X() + distance * cos(oGame->Deg_To_Rad((float)rot)));
    Set_Y(Get_Y() + distance * sin(oGame->Deg_To_Rad((float)rot)));
}
