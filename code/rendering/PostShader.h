/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Nov 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _POST_SHADER_H_
#define _POST_SHADER_H_

/**
 * PostShader object header.
 */


// Includes
#include <string>
#include "Shader.h"


// Forward declaration
class Shader;


/**
 * PostShaders are used as post-processing for render layers.
 * Most post processing shaders will probably extend from this one to
 * add additional control over uniforms etc.
 */
class PostShader : public Shader
{

public:
	PostShader(std::string shader_file_name);
	void Get_Uniform_Locations();
	virtual void Set_Uniform_Values(int pass);
	void Specify_Vertex_Layout();
	virtual void Setup(int pass);
	void Cleanup();
	virtual int Num_Passes_Required();
	virtual bool Should_Apply();

private:

	GLint oAttribute_Vertex_Coord;
	GLint oAttribute_Texture_Coord;

};


#endif