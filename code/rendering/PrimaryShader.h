/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _PRIMARY_SHADER_H_
#define _PRIMARY_SHADER_H_

/**
 * PrimaryShader object header.
 */


// Includes
#include <string>
#include "Shader.h"


// Forward declaration


/**
 * Primary shaders are used for drawing Entities directly
 * to RenderLayers as opposed to PostProcessingShaders which are
 * used to alter the fragments of already drawn layers.
 */
class PrimaryShader : public Shader
{

public:
	PrimaryShader(std::string shader_file_name);
	void Get_Uniform_Locations();
	void Set_Uniform_Values();
	void Setup();
	void Cleanup();

private:

	GLint oAttribute_Vertex_Coord;
	GLint oAttribute_Vertex_Colour;
	GLint oAttribute_Texture_Coord;
	GLint oAttribute_Pos_Rotation_Scale;

};


#endif