/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Jul 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _RENDER_MODE_H_
#define _RENDER_MODE_H_

/**
 * RenderMode object header.
 */


// Includes
#include <map>
#include <string>
#include <GL/glew.h>
#include <GL/glu.h>
#include "Entity.h"
#include "Game.h"


/**
 * When created this object loads the relevant shader for itself.
 * Then it is responsible for drawing objects during a render pass
 * it is passed the objects to draw.
 */
class RenderMode
{

private:
	GLuint oVertex_Shader_Program;
	GLuint oFragment_Shader_Program;
	GLuint oShader_Program;
	Game* oGame;
	std::map<std::string, GLint> oUniforms;
	GLint oAttribute_Vertex_Coord;
	GLint oAttribute_Vertex_Colour;
	GLint oAttribute_Texture_Coord;
	GLint oAttribute_Pos_Rotation_Scale;

public:
	RenderMode(std::string shader_file_name);
	~RenderMode();
	virtual void Get_Uniform_Locations();
	virtual void Set_Uniform_Values();
	virtual void Setup();
	virtual void Cleanup();

};

#endif