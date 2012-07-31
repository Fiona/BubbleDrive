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
	aPosition[0] = 0.0f; aPosition[1] = 0.0f; aPosition[2] = -100.0f; aPosition[3] = 0.0f;
    oImage = NULL;
    oGame = Game::Instance();
    oGame->Register_Entity(this);

	// GL stuff
	glGenBuffers(3, aVBO);

	Create_Colour_Array();
	Create_Vertex_Array();
	Create_Texture_Coord_Array();

}


/**
 * Deconstructor
 */
Entity::~Entity()
{
	glDeleteBuffers(3, aVBO);
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
 * This is called for every Entity as much as possible to while ensuring
 * a steady framerate.
 * By default it will draw at the right position, rotation, scale etc.
 * It's designed to be overridden if necessary to add some custom drawing.
 */
void Entity::Draw()
{

    // Bind texture coords if our image is a multi frame one
	/*
    if(Get_Image()->iFrame_Count > 1)
        glTexCoordPointer(2, GL_FLOAT, 0, &Get_Image()->aTexture_Coord_Lists[iImage_Frame - 1][0]);
	*/

    // Binding texture
    if(oGame->iCurrent_Bound_Texture != Get_Image()->iTexture_Num)
    {
	    glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Get_Image()->iTexture_Num);
        oGame->iCurrent_Bound_Texture = Get_Image()->iTexture_Num;
    }

	glUseProgram(oGame->oShader_Program);
    glUniform1i(oGame->iUniform_Texture_Num, 0);
	glUniform4fv(oGame->iUniform_Position, 1, aPosition);
	glUniform1f(oGame->iUniform_Rotation, fRotation);
	glUniform1f(oGame->iUniform_Scale, fScale);
	glUniform4fv(oGame->iUniform_Camera_Position, 1, oGame->aCamera_Position);
	float screen_size[2]; screen_size[0] = DEFAULT_SCREEN_WIDTH; screen_size[1] = DEFAULT_SCREEN_HEIGHT;
	glUniform2fv(oGame->iUniform_Screen_Size, 1, screen_size);

	/**/
	glBindBuffer(GL_ARRAY_BUFFER, aVBO[1]);
	glColorPointer(4, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, aVBO[0]);
	glVertexPointer(2, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, aVBO[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	//glTexCoordPointer(2, GL_FLOAT, 0, &Get_Image()->aTexture_Coord_Lists[iImage_Frame - 1][0]);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY); 

	glUseProgram(0);

	/**/

	/*
    // Reset the texture coords back to default if necessary
    if(Get_Image()->iFrame_Count > 1)
        glTexCoordPointer(2, GL_FLOAT, 0, &oGame->oDefault_Texture_Coords[0]);
	*/

}


/**
 * This should be used to kill an entity and stop them from being executed and drawn.
 * It's designed to be overridden if necessary to add some additional behaviour upon killing.
 */
void Entity::Kill()
{

   oGame->Unregister_Entity(this);

}


/**
 * Used to make the Vertex location VBO up to date.
 */
void Entity::Create_Vertex_Array()
{

	// The offset values are for calculating the centre of the triangles
	// which should be 0,0. Rather than 0,0 being the top left.
	float w, h, w_offset, h_offset;

	if(Get_Image() == NULL)
	{

		w = 0.0f;
		h = 0.0f;
		w_offset = 0.0f;
		h_offset = 0.0f;

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

		w_offset = w/2;
		h_offset = h/2;

	}

	GLsizeiptr Vertex_Size = 6 * 2 * sizeof(GLfloat);
	// tri 1 top right
	aVertex_Data[0] = w - w_offset; aVertex_Data[1] = -h_offset;
	// tri 1 top left
	aVertex_Data[2] = -w_offset; aVertex_Data[3] = -h_offset;
	// tri 1 bottom left
	aVertex_Data[4] = -w_offset; aVertex_Data[5] = h - h_offset;

	// tri 2 bottom left
	aVertex_Data[6] = -w_offset; aVertex_Data[7] = h - h_offset;
	// tri 2 bottom right
	aVertex_Data[8] = w - w_offset; aVertex_Data[9] = h - h_offset;
	// tri 2 top right
	aVertex_Data[10] = w - w_offset; aVertex_Data[11] = -h_offset;

	glBindBuffer(GL_ARRAY_BUFFER, aVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, Vertex_Size, aVertex_Data, GL_DYNAMIC_DRAW);

}


/**
 * Used to make the Colour data VBO up to date.
 */
void Entity::Create_Colour_Array()
{

	GLsizeiptr Colour_Size = 6 * 4 * sizeof(GLfloat);
	aColour_Data[0] = aColour[0];  aColour_Data[1] = aColour[1];  aColour_Data[2] = aColour[2];  aColour_Data[3] = fAlpha;
	aColour_Data[4] = aColour[0];  aColour_Data[5] = aColour[1];  aColour_Data[6] = aColour[2];  aColour_Data[7] = fAlpha;
	aColour_Data[8] = aColour[0];  aColour_Data[9] = aColour[1];  aColour_Data[10] = aColour[2]; aColour_Data[11] = fAlpha;
	aColour_Data[12] = aColour[0]; aColour_Data[13] = aColour[1]; aColour_Data[14] = aColour[2]; aColour_Data[15] = fAlpha;
	aColour_Data[16] = aColour[0]; aColour_Data[17] = aColour[1]; aColour_Data[18] = aColour[2]; aColour_Data[19] = fAlpha;
	aColour_Data[20] = aColour[0]; aColour_Data[21] = aColour[1]; aColour_Data[22] = aColour[2]; aColour_Data[23] = fAlpha;

	glBindBuffer(GL_ARRAY_BUFFER, aVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, Colour_Size, aColour_Data, GL_DYNAMIC_DRAW);

}


/**
 * Used to make the Texture coord data VBO up to date.
 */
void Entity::Create_Texture_Coord_Array()
{

	for(int i = 0; i < 12; i++)
		aText_Coord_Data[i] = 0.0f;

	GLsizeiptr Text_Coord_Size = 6 * 2 * sizeof(GLfloat);

	if(Get_Image() == NULL)
	{
		glBindBuffer(GL_ARRAY_BUFFER, aVBO[2]);
		glBufferData(GL_ARRAY_BUFFER, Text_Coord_Size, aText_Coord_Data, GL_DYNAMIC_DRAW);
		return;
	}

	float sequence_start_pos, texture_x_from, texture_x_to;

    if(Get_Image()->iFrame_Count > 1)
        sequence_start_pos = 0.01f * (((float)Get_Image()->iWidth / (float)Get_Image()->iRaw_Surface_Width) * 100.0f);
    else
        sequence_start_pos = 0.0f;

	if(Get_Image()->iFrame_Count == 1)
	{
		texture_x_from = 0.0f;
        texture_x_to = 1.0;
	}
    else
    {
		texture_x_from = sequence_start_pos * Get_Image_Frame();
        texture_x_to = (sequence_start_pos * Get_Image_Frame()) + sequence_start_pos;
	}

    aText_Coord_Data[0] = texture_x_to;
    aText_Coord_Data[2] = texture_x_from;
    aText_Coord_Data[4] = texture_x_from; aText_Coord_Data[5] = 1.0f;
    aText_Coord_Data[6] = texture_x_from; aText_Coord_Data[7] = 1.0f;
    aText_Coord_Data[8] = texture_x_to; aText_Coord_Data[9] = 1.0f;
    aText_Coord_Data[10] = texture_x_to;

	glBindBuffer(GL_ARRAY_BUFFER, aVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, Text_Coord_Size, aText_Coord_Data, GL_DYNAMIC_DRAW);

}


/**
 * Basic Getters/Setters
 * Can all be overriden.
 */
void Entity::Set_X(float X)
{
    fX = X;
	if(oImage == NULL)
		aPosition[0] = X;
	else
		aPosition[0] = X - (float)(oImage->iWidth/2);
}

float Entity::Get_X()
{
	return fX;
}

void Entity::Set_Y(float Y)
{
    fY = Y;
	if(oImage == NULL)
		aPosition[1] = Y;
	else
		aPosition[1] = Y - (float)(oImage->iHeight/2);
	if(Get_Render_Mode() == RENDER_MODE_SCREEN)
	{
		aPosition[0] /= (float)((float)OPTIMAL_SCREEN_WIDTH / (float)DEFAULT_SCREEN_WIDTH);
		aPosition[1] /= (float)((float)OPTIMAL_SCREEN_HEIGHT / (float)DEFAULT_SCREEN_HEIGHT);
	}
}

float Entity::Get_Y()
{
	return fY;
 }

void Entity::Set_Z(float Z)
{
    iZ = Z;
	aPosition[2] = 0.0f;
}

float Entity::Get_Z()
{
    return iZ;
}

void Entity::Set_Rotation(float Rotation)
{
    fRotation = Rotation;
}

float Entity::Get_Rotation()
{
    return fRotation;
}

void Entity::Set_Scale(float Scale)
{
    fScale = Scale;
}

float Entity::Get_Scale()
{
    return fScale;
}

void Entity::Set_Image(Image* Image)
{
    oImage = Image;
	Create_Vertex_Array();
	Create_Texture_Coord_Array();
}

Image* Entity::Get_Image()
{
    return oImage;
}

void Entity::Set_Image_Frame(int image_frame)
{
    iImage_Frame = image_frame;
}

int Entity::Get_Image_Frame()
{
    return iImage_Frame;
}

void Entity::Set_Render_Mode(int render_mode)
{
	iRender_Mode = render_mode;
}

int Entity::Get_Render_Mode()
{
	return iRender_Mode;
}

void Entity::Set_Colour(float r, float g, float b)
{
    aColour[0] = r;
    aColour[1] = g;
    aColour[2] = b;
	Create_Colour_Array();
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

		if(Get_Image())
		{
			hotspot_pos[0] -= Get_Image()->iWidth/2;
			hotspot_pos[1] -= Get_Image()->iHeight/2;
		}

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
