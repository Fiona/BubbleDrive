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
#include "Game.h"
#include "Text.h"


/**
 * Contstructor.
 */
Text::Text() : Entity()
{

	sText = "";
	oFont = 0;
	iSize = 0;
	bReady = false;

}



/**
 * Draw method
 */
void Text::Draw()
{

	if(sText == "")
		return;

	if(!bReady)
		Ready();

    glPushMatrix();


    float X = Get_X();
    float Y = Get_Y();
    glTranslatef(X, Y + fTallest_Y_Bearing, 0.0f);
	
	std::pair<GlyphMap*, std::vector<float>* > p;
	BOOST_FOREACH(p, oVertex_List) 
	{
	
		glTexCoordPointer(2, GL_FLOAT, 0, &oTexture_Coords.find(p.first)->second[0][0]);
		glVertexPointer(2, GL_FLOAT, 0, &p.second[0][0]);

		// Binding texture
	    if(oGame->iCurrent_Bound_Texture != p.first->iTexture)
		{	
	        glBindTexture(GL_TEXTURE_2D, p.first->iTexture);
	        oGame->iCurrent_Bound_Texture = p.first->iTexture;
		}

		// Colour and transparency
	    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		// draw the triangle strip
		glDrawArrays(GL_TRIANGLES, 0, (int)(p.second->size()/2));

	}

	glTexCoordPointer(2, GL_FLOAT, 0, &oGame->oDefault_Texture_Coords[0]);

    glPopMatrix();

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
 * Readys the text for drawing, using the current text/font/size etc
 * settings this will build the vertex and texture coordinate lists that
 * will be used to draw the text to the screen.
 */
void Text::Ready()
{

	if(bReady)
		return;

	// Clear up lists so we can go again
	oVertex_List.clear();
	oTexture_Coords.clear();

	// For each character in the string we have to request a pointer to the 
	// Glyph object so we can gather it's information and metrics.
	Glyph* current_glyph;
	float hor_draw_position = 0.0f;
	std::map<GlyphMap*, std::vector<float>* >::iterator glyphbin_search_it;
	std::vector<float>* vertex_vector;
	std::vector<float>* texture_coord_vector;
	fTallest_Y_Bearing = 0.0f;

	for(int i = 0; i < (int)sText.length(); i++)
	{

		current_glyph = 0;
		current_glyph = oGame->oFont_Manager->Request_Glyph(
			oFont,
			iSize,
			sText.substr(i, 1)
			);

		if(current_glyph == 0)
			continue;

		if(current_glyph->fY_Bearing > fTallest_Y_Bearing)
			fTallest_Y_Bearing = current_glyph->fY_Bearing;

		// If we have no knowledge of the glyphbin in question then we add it.
		glyphbin_search_it = oVertex_List.find(oGame->oFont_Manager->oGlyph_Maps[current_glyph->iGlyph_Map]);

		if(glyphbin_search_it != oVertex_List.end())
		{
			vertex_vector = glyphbin_search_it->second;
			texture_coord_vector = oTexture_Coords.find(oGame->oFont_Manager->oGlyph_Maps[current_glyph->iGlyph_Map])->second;
		}
		else
		{
			vertex_vector = new std::vector<float>;
			texture_coord_vector = new std::vector<float>;
			oVertex_List.insert(
				std::pair<GlyphMap*, std::vector<float>* >(oGame->oFont_Manager->oGlyph_Maps[current_glyph->iGlyph_Map], vertex_vector)
				);
			oTexture_Coords.insert(
				std::pair<GlyphMap*, std::vector<float>* >(oGame->oFont_Manager->oGlyph_Maps[current_glyph->iGlyph_Map], texture_coord_vector)
				);
		}

		// Add glyph to vertex list
		// tri 1 top right		
		vertex_vector->push_back(current_glyph->fWidth + hor_draw_position); vertex_vector->push_back(0.0f - current_glyph->fY_Bearing); 
		// tri 1 top left
		vertex_vector->push_back(hor_draw_position); vertex_vector->push_back(0.0f - current_glyph->fY_Bearing); 
		// tri 1 bottom left
		vertex_vector->push_back(hor_draw_position); vertex_vector->push_back(current_glyph->fHeight - current_glyph->fY_Bearing);

		// tri 2 bottom left
		vertex_vector->push_back(hor_draw_position); vertex_vector->push_back(current_glyph->fHeight - current_glyph->fY_Bearing);
		// tri 2 bottom right
		vertex_vector->push_back(current_glyph->fWidth + hor_draw_position); vertex_vector->push_back(current_glyph->fHeight - current_glyph->fY_Bearing); 
		// tri 2 top right
		vertex_vector->push_back(current_glyph->fWidth + hor_draw_position); vertex_vector->push_back(0.0f - current_glyph->fY_Bearing); 
		
		// Add to texture coords list
		// tri 1 top right
		texture_coord_vector->push_back(0.01f * ((((float)current_glyph->oNode->iX + (float)current_glyph->oNode->iWidth) / GLYPH_MAP_WIDTH) * 100.0f));
		texture_coord_vector->push_back(0.01f * (((float)current_glyph->oNode->iY / GLYPH_MAP_HEIGHT) * 100.0f));
		// tri 1 top left
		texture_coord_vector->push_back(0.01f * (((float)current_glyph->oNode->iX / GLYPH_MAP_WIDTH) * 100.0f));
		texture_coord_vector->push_back(0.01f * (((float)current_glyph->oNode->iY / GLYPH_MAP_HEIGHT) * 100.0f));
		// tri 1 bottom left
		texture_coord_vector->push_back(0.01f * (((float)current_glyph->oNode->iX / GLYPH_MAP_WIDTH) * 100.0f));
		texture_coord_vector->push_back(0.01f * ((((float)current_glyph->oNode->iY + (float)current_glyph->oNode->iHeight) / GLYPH_MAP_HEIGHT) * 100.0f));

		// tri 2 bottom left
		texture_coord_vector->push_back(0.01f * (((float)current_glyph->oNode->iX / GLYPH_MAP_WIDTH) * 100.0f));
		texture_coord_vector->push_back(0.01f * ((((float)current_glyph->oNode->iY + (float)current_glyph->oNode->iHeight) / GLYPH_MAP_HEIGHT) * 100.0f));
		// tri 2 bottom right
		texture_coord_vector->push_back(0.01f * ((((float)current_glyph->oNode->iX + (float)current_glyph->oNode->iWidth) / GLYPH_MAP_WIDTH) * 100.0f));
		texture_coord_vector->push_back(0.01f * ((((float)current_glyph->oNode->iY + (float)current_glyph->oNode->iHeight) / GLYPH_MAP_HEIGHT) * 100.0f));
		// tri 2 top right
		texture_coord_vector->push_back(0.01f * ((((float)current_glyph->oNode->iX + (float)current_glyph->oNode->iWidth) / GLYPH_MAP_WIDTH) * 100.0f));
		texture_coord_vector->push_back(0.01f * (((float)current_glyph->oNode->iY / GLYPH_MAP_HEIGHT) * 100.0f));

		hor_draw_position += current_glyph->fAdvance;

	}

	bReady = true;

}