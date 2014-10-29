/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started May 2012 by Fiona ****
 ***************************************/

/**
 * Font Manager object source file.
 */


// Includes
#include "FontManager.h"
#include "GlyphMap.h"
#include "Font.h"
#include "Glyph.h"


/**
 * Contstructor. Initialises FT2 library and creates first GlyphMap
 */
FontManager::FontManager()
{

	// Init FT2
	if(FT_Init_FreeType(&oFTLib))
	{
		std::cout << "Error initialising Freetype2." << std::endl;
		return;
	}

	oGlyphs.clear();
	oGlyph_Maps.clear();

	// Create first GlyphMap
	oGlyph_Maps.push_back(new GlyphMap);

}


/**
 * Deconstructor
 */
FontManager::~FontManager()
{

	FT_Done_FreeType(oFTLib);
	oGlyph_Maps.clear();

    for(std::map<GlyphKey, Glyph*>::iterator it = oGlyphs.begin(); it != oGlyphs.end(); ++it)
    {
        if(it->second == NULL)
            continue;
        delete(it->second);
    }

}


/**
 * Returns a Glyph object relating to the details asked for. 
 * If there was a problem returning the Glyph 0 will be returned.
 */
Glyph* FontManager::Request_Glyph(Font* font, int size, std::string character)
{

	// If we already have created this Glyph object and it's cached we 
	// return that object.
	std::map<GlyphKey, Glyph*>::iterator glyph_search_it;
	glyph_search_it = oGlyphs.find(GlyphKey(font, size, character) );

	if(glyph_search_it != oGlyphs.end())
		return glyph_search_it->second;

	// Glyph clearly does not exist, therefore we create a new Glyph object
	Glyph* new_glyph = new Glyph(character, font, size);
	oGlyphs.insert(std::pair<GlyphKey, Glyph*>(GlyphKey(font, size, character), new_glyph));
	Place_Glyph_In_GlyphBin(new_glyph);

	return new_glyph;

}


/**
 * Inserts a given Glyph into a GlyphBin
 * It alters the Glyph to assgn it's bin number and Node object to it.
 */
bool FontManager::Place_Glyph_In_GlyphBin(Glyph* glyph)
{

	// Can assume that the loaded glyph in the FT2 glyphslot is the last added one
	// so we will give it's bitmap to the GlyphBin to add it to the texture.
    FT_Glyph glyphDesc;
	FT_Get_Glyph(glyph->oFont->oFace->glyph, &glyphDesc);

    FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, 0, 1);
    FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyphDesc;
    FT_Bitmap& bitmap = bitmapGlyph->bitmap;

	// MAKES IT BOOOLD
	//FT_Pos weight = 1 << 6;
	//FT_Bitmap_Embolden(oFTLib, &bitmap, weight, weight);

	int width = bitmap.width;
	int height = bitmap.rows;
	glyph->fWidth = (float)width; glyph->fHeight = (float)height;

	std::vector<unsigned char> pixel_buffer;
	pixel_buffer.clear();
	pixel_buffer.resize((int)(width * height * 4), 255);
	unsigned char* pixels = bitmap.buffer;

    if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
    {
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
            {
				std::size_t index = (x + y * width) * 4 + 3;
                pixel_buffer[index] = ((pixels[x / 8]) & (1 << (7 - (x % 8)))) ? 255 : 0;
            }
            pixels += bitmap.pitch;
        }
    }
    else
    {
		for(int y = 0; y < height; ++y)
		{	
			for(int x = 0; x < width; ++x)
			{
				std::size_t index = (int)((x + y * width) * 4 + 3);
				pixel_buffer[index] = pixels[x];
			}
			pixels += bitmap.pitch;
		}
	}

	// We try to insert the Glyph into every GlyphMap and set
	// the various settings on the Glyph itself.
	RectangleBin::Node* new_node = 0;
	int glyphmap_num = -1;

    BOOST_FOREACH(GlyphMap* glyphmap, oGlyph_Maps)
    {

		glyphmap_num++;

		new_node = glyphmap->Insert((int)glyph->fWidth, (int)glyph->fHeight);
		if(new_node != 0)
			break;
        
    }

	// If we've not found a suitable glyph map then we need to create a new one
	if(new_node == 0)
	{
		GlyphMap* new_glyphmap = new GlyphMap;
		oGlyph_Maps.push_back(new_glyphmap);
		glyphmap_num = (int)oGlyph_Maps.size() - 1;
		new_node = new_glyphmap->Insert((int)glyph->fWidth, (int)glyph->fHeight);
	}

	if(new_node == 0)
		return false;

	// Set the Glyph settings based on this stuff
	glyph->iGlyph_Map = glyphmap_num;
	glyph->oNode = new_node;

	// If the character has no presence then we can skip drawing bitmaps
	if(glyph->fWidth <= 0.0f || glyph->fHeight <= 0.0f)
		return true;

	// Finally add it to the glyph map texture
	oGlyph_Maps[glyphmap_num]->Add_Bitmap(&pixel_buffer[0], new_node);
	
	return true;

}
