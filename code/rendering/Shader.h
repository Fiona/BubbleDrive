/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

class Shader;

#pragma once
#ifndef _SHADER_H_
#define _SHADER_H_

/**
 * Shader object header.
 */


// Includes
#include <map>
#include <string>
#include <GL/glew.h>
#include "../core/Game.h"
#include "../core/consts.h"


/**
 * Shaders load the text files that make up shaders and 
 * compile them. They're then assigned to RenderLayer's as
 * primary shaders or post processing shaders.
 */
class Shader
{

public:
	Game* oGame;
	GLuint oShader_Program;
	std::map<std::string, GLint> oUniforms;

	Shader(std::string shader_file_name);
	~Shader();
	virtual void Get_Uniform_Locations();
	virtual void Set_Uniform_Values();
	virtual void Specify_Vertex_Layout();
	virtual void Setup();
	virtual void Cleanup();

private:

	GLuint oGeometry_Shader_Program;
	GLuint oVertex_Shader_Program;
	GLuint oFragment_Shader_Program;

    bool load_from_file(GLenum shader_type, std::string shader_source_file, GLuint &shader_program);

};


#endif
