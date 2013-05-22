/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started May 2012 by Fiona ****
 ***************************************/

/**
 * Text source file.
 */


// Includes
#include "../core/Game.h"
#include "Text.h"


/**
 * Contstructor.
 */
Text::Text() : Entity()
{

	sText = "";
	oFont = 0;
	iSize = 0;
	fLine_Height_Padding = 0.0f;
	bReady = false;

}


/**
 * Empty deconstructor to satisfy virtual table
 */
Text::~Text()
{

}


/*
 * Setting colour
 */
void Text::Set_Colour(float r, float g, float b)
{
	Entity::Set_Colour(r, g, b);
	bReady = false;
}


/**
 * Set the text string to draw to the screen.
 */
void Text::Set_Text(std::string text)
{
	sText = text;
	bReady = false;
}


/**
 * Sets the Font object to use when drawing to the screen.
 */
void Text::Set_Font(Font* font)
{
	oFont = font;
	bReady = false;
}


/** 
 * Sets the font size to draw this text with.
 */
void Text::Set_Size(int size)
{
	iSize = size;
	bReady = false;
}


/** 
 * Sets how much of an extra gap there should be between lines.
 */
void Text::Set_Line_Height_Padding(float line_height_padding)
{
	fLine_Height_Padding = line_height_padding;
	bReady = false;
}


/**
 * Readys the text for drawing, using the current text/font/size etc
 * settings this will build the vertex and texture coordinate lists that
 * will be used to draw the text to the screen.
 */
void Text::Ready_For_Rendering()
{

	if(bReady)
		return;

	// For each character in the string we have to request a pointer to the 
	// Glyph object so we can gather it's information and metrics.
	Glyph* current_glyph;
	float hor_draw_position = 0.0f;
	float ver_draw_position = 0.0f;
	float tallest_glyph = 0.0f;
	FT_Bool use_kerning = FT_HAS_KERNING(oFont->oFace);
	Glyph* previous_glyph = 0;
	fTallest_Y_Bearing = 0.0f;
	iNum_Objects = 0;
	oText_Characters.clear();

	float w_adjust = 1.0f;
	float h_adjust = 1.0f;
	if(Get_Render_Layer() == RENDER_LAYER_SCREEN)
	{
		w_adjust = (float)((float)OPTIMAL_SCREEN_WIDTH / (float)DEFAULT_SCREEN_WIDTH);
		h_adjust = (float)((float)OPTIMAL_SCREEN_HEIGHT / (float)DEFAULT_SCREEN_HEIGHT);
	}

	for(int i = 0; i < (int)sText.length(); i++)
	{

		if(sText.substr(i, 1) == "\n")
		{
			ver_draw_position += (tallest_glyph / h_adjust) + (fLine_Height_Padding / h_adjust);
			hor_draw_position = 0.0f;
			continue;
		}

		current_glyph = 0;
		current_glyph = oGame->oFont_Manager->Request_Glyph(
			oFont,
			iSize,
			sText.substr(i, 1)
			);

		if(current_glyph == 0)
		{
			previous_glyph = 0;
			continue;
		}

		if(previous_glyph && use_kerning)
			hor_draw_position += current_glyph->Get_Kerning(previous_glyph) / w_adjust;
		previous_glyph = current_glyph;

		if(current_glyph->fY_Bearing > fTallest_Y_Bearing)
			fTallest_Y_Bearing = current_glyph->fY_Bearing;

		if(current_glyph->fHeight > tallest_glyph)
			tallest_glyph = current_glyph->fHeight;

		if(current_glyph->fWidth <= 0.0f || current_glyph->fHeight <= 0.0f)
		{
			hor_draw_position += current_glyph->fAdvance / w_adjust;
			continue;
		}

		iNum_Objects++;

		oText_Characters.push_back(
			new TextCharacter(current_glyph, hor_draw_position, ver_draw_position, oGame->oFont_Manager->oGlyph_Maps[current_glyph->iGlyph_Map])
			);

		hor_draw_position += current_glyph->fAdvance / w_adjust;

	}

    BOOST_FOREACH(TextCharacter* character, oText_Characters)
    {

		character->fY_Pos += tallest_glyph / h_adjust;

    }

	bReady = true;
	Update_Batches_And_Object_Indicies(true);

}


/**
 * Used to get the glyphbin texture num for specific glyphs (objects)
 */
int Text::Get_Texture_Num_For_Object_Num(int obj_num)
{

	if(oText_Characters.size() == 0)
		return 0;
	return oText_Characters[obj_num - 1]->oGlyph_Map->iTexture;

}


/**
 * Returns the vbo data for a specific text character
 */
void Text::Get_Object_Index_Data(int object_index, GLfloat* vbo_data, int entity_object_num)
{

	if(oText_Characters.size() == 0)
		return;

	TextCharacter* character = oText_Characters[entity_object_num - 1];

	// *******
	// Width / Height
	// *******
	float w_adjust = 1.0f;
	float h_adjust = 1.0f;
	if(Get_Render_Layer() == RENDER_LAYER_SCREEN)
	{
		w_adjust = (float)((float)OPTIMAL_SCREEN_WIDTH / (float)DEFAULT_SCREEN_WIDTH);
		h_adjust = (float)((float)OPTIMAL_SCREEN_HEIGHT / (float)DEFAULT_SCREEN_HEIGHT);
	}

	float ch_x, ch_y, w, h;
	ch_x = character->fX_Pos;
	w = character->oGlyph->fWidth / w_adjust;
	ch_y = character->fY_Pos - (character->oGlyph->fY_Bearing / h_adjust);
	h = character->oGlyph->fHeight / h_adjust;

	// *******
	// Texture pos
	// *******
	float texture_x_from, texture_x_to, texture_y_from, texture_y_to;

	texture_x_from = 0.01f * (((float)character->oGlyph->oNode->iX / GLYPH_MAP_WIDTH) * 100.0f);
	texture_x_to = 0.01f * ((((float)character->oGlyph->oNode->iX + (float)character->oGlyph->oNode->iWidth - 1) / GLYPH_MAP_WIDTH) * 100.0f);
	texture_y_from = 0.01f * (((float)character->oGlyph->oNode->iY / GLYPH_MAP_HEIGHT) * 100.0f);
	texture_y_to = 0.01f * ((((float)character->oGlyph->oNode->iY + (float)character->oGlyph->oNode->iHeight - 1) / GLYPH_MAP_HEIGHT) * 100.0f);

	// *******
	// extra data
	// *******
	float x, y, rot, scale, alpha;
	x = Get_X();
	y = Get_Y();
	rot = Get_Rotation();
	scale = Get_Scale();
	alpha = Get_Alpha();


	if(Get_Render_Layer() == RENDER_LAYER_SCREEN)
	{
		x /= (float)((float)OPTIMAL_SCREEN_WIDTH / (float)DEFAULT_SCREEN_WIDTH);
		y /= (float)((float)OPTIMAL_SCREEN_HEIGHT / (float)DEFAULT_SCREEN_HEIGHT);
	}

	// *******
	// Data creation
	// *******
	int vjump = 0;

	//  ---- VERTEX LOCS -------------- COLOURS -------------------------- TEXTURE COORDS -------------- POS/ROTATION/SCALING

	// tri 1 top right	
	vbo_data[vjump] = ch_x + w; vbo_data[vjump+2] = aColour[0];	vbo_data[vjump+6] = texture_x_to;	vbo_data[vjump+8] = x;
	vbo_data[vjump+1] = ch_y;	vbo_data[vjump+3] = aColour[1];	vbo_data[vjump+7] = texture_y_from;	vbo_data[vjump+9] = y;
								vbo_data[vjump+4] = aColour[2];										vbo_data[vjump+10] = rot;
								vbo_data[vjump+5] = alpha;											vbo_data[vjump+11] = scale;

	// tri 1 top left
	vjump += NUM_ELEMENTS_PER_VERTEX;
	vbo_data[vjump] = ch_x; 	vbo_data[vjump+2] = aColour[0];	vbo_data[vjump+6] = texture_x_from;	vbo_data[vjump+8] = x;
	vbo_data[vjump+1] = ch_y;	vbo_data[vjump+3] = aColour[1];	vbo_data[vjump+7] = texture_y_from;	vbo_data[vjump+9] = y;
								vbo_data[vjump+4] = aColour[2];										vbo_data[vjump+10] = rot;
								vbo_data[vjump+5] = alpha;											vbo_data[vjump+11] = scale;

	// tri 1 bottom left
	vjump += NUM_ELEMENTS_PER_VERTEX;
	vbo_data[vjump] = ch_x;			vbo_data[vjump+2] = aColour[0];	vbo_data[vjump+6] = texture_x_from;	vbo_data[vjump+8] = x;
	vbo_data[vjump+1] = ch_y + h;	vbo_data[vjump+3] = aColour[1];	vbo_data[vjump+7] = texture_y_to;	vbo_data[vjump+9] = y;
									vbo_data[vjump+4] = aColour[2];										vbo_data[vjump+10] = rot;
									vbo_data[vjump+5] = alpha;											vbo_data[vjump+11] = scale;

	// tri 2 bottom left
	vjump += NUM_ELEMENTS_PER_VERTEX;
	vbo_data[vjump] = ch_x; 		vbo_data[vjump+2] = aColour[0];	vbo_data[vjump+6] = texture_x_from;	vbo_data[vjump+8] = x;
	vbo_data[vjump+1] = ch_y + h;	vbo_data[vjump+3] = aColour[1];	vbo_data[vjump+7] = texture_y_to;	vbo_data[vjump+9] = y;
									vbo_data[vjump+4] = aColour[2];										vbo_data[vjump+10] = rot;
									vbo_data[vjump+5] = alpha;											vbo_data[vjump+11] = scale;

	// tri 2 bottom right
	vjump += NUM_ELEMENTS_PER_VERTEX;
	vbo_data[vjump] = ch_x + w; 	vbo_data[vjump+2] = aColour[0];	vbo_data[vjump+6] = texture_x_to;	vbo_data[vjump+8] = x;
	vbo_data[vjump+1] = ch_y + h;	vbo_data[vjump+3] = aColour[1];	vbo_data[vjump+7] = texture_y_to;	vbo_data[vjump+9] = y;
									vbo_data[vjump+4] = aColour[2];										vbo_data[vjump+10] = rot;
									vbo_data[vjump+5] = alpha;											vbo_data[vjump+11] = scale;

	// tri 2 top right
	vjump += NUM_ELEMENTS_PER_VERTEX;
	vbo_data[vjump] = ch_x + w; vbo_data[vjump+2] = aColour[0];	vbo_data[vjump+6] = texture_x_to;	vbo_data[vjump+8] = x;
	vbo_data[vjump+1] = ch_y;	vbo_data[vjump+3] = aColour[1];	vbo_data[vjump+7] = texture_y_from;	vbo_data[vjump+9] = y;
								vbo_data[vjump+4] = aColour[2];										vbo_data[vjump+10] = rot;
								vbo_data[vjump+5] = alpha;											vbo_data[vjump+11] = scale;

}