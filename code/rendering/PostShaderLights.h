/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Nov 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _POST_SHADER_LIGHTS_H_
#define _POST_SHADER_LIGHTS_H_

/**
 * PostShaderLights object header.
 */


// Includes
#include <string>
#include "PostShader.h"


// Forward declaration
class PostShader;


/**
 * This is a special version of the PostShader that
 * draws light effects on to render layers.
 */
class PostShaderLights : public PostShader
{

public:
	PostShaderLights(std::string shader_file_name);
	void Get_Uniform_Locations();
	void Set_Uniform_Values(int pass);
	void Specify_Vertex_Layout();
	void Setup(int pass);
	void Cleanup();
	int Num_Passes_Required();
	bool Should_Apply();

};


#endif