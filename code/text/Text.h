/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started May 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _TEXT_H_
#define _TEXT_H_


// Includes
#include <vector>
#include <map>
#include <string>
#include <boost/foreach.hpp>
#include "../core/consts.h"
#include "../core/Entity.h"
#include "GlyphMap.h"
#include "Font.h"
#include "TextCharacter.h"


// Forward declaration
class Entity;


/**
 * Text objects will draw texts to the screen. If settings
 * are changed using the Set_* methods, Ready() must be called
 * before it can be drawn.
 */
class Text : public Entity
{

public:
	Text();
	~Text();

	void Set_Colour(float r, float g, float b);
	void Set_Text(std::string text);
	void Set_Font(Font* font);
	void Set_Size(int size);
	void Set_Line_Height_Padding(float line_height_padding);
	void Ready_For_Rendering();
	int Get_Texture_Num_For_Object_Num(int obj_num);
	void Get_Object_Index_Data(int object_index, GLfloat* vbo_data, int entity_object_num);

private:
	std::string sText;
	Font* oFont;
	int iSize;
	float fLine_Height_Padding;
	bool bReady;
	float fTallest_Y_Bearing;
	std::vector<TextCharacter*> oText_Characters;

};

#endif