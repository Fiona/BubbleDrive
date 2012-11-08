/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Oct 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _RENDER_LAYER_SHADER_H_
#define _RENDER_LAYER_SHADER_H_

/**
 * RenderLayerShader object header.
 */


// Includes
#include <string>
#include "Shader.h"


// Forward declaration
class Shader;


/**
 * RenderLayerShaders are very simple shaders that render a screen sized
 * quad with a single texture without anything special going on.
 */
class RenderLayerShader : public Shader
{

public:
	RenderLayerShader(std::string shader_file_name);
	void Get_Uniform_Locations();
	void Set_Uniform_Values();
	void Specify_Vertex_Layout();
	void Setup();
	void Cleanup();

private:

	GLint oAttribute_Vertex_Coord;
	GLint oAttribute_Texture_Coord;

};


#endif