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
#include "consts.h"
#include "Entity.h"
#include "GlyphMap.h"
#include "Font.h"

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

	void Draw();
	void Set_Colour(float r, float g, float b);
	void Set_Text(std::string text);
	void Set_Font(Font* font);
	void Set_Size(int size);
	void Ready();

private:
	std::string sText;
	Font* oFont;
	int iSize;
	bool bReady;
	std::map<GlyphMap*, std::vector<float>* > oVertex_List;
	std::map<GlyphMap*, std::vector<float>* > oTexture_Coords;
	float fTallest_Y_Bearing;

};

#endif