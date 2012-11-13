/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Nov 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _POST_SHADER_BLUR_H_
#define _POST_SHADER_BLUR_H_

/**
 * PostShaderBlur object header.
 */


// Includes
#include <string>
#include "PostShader.h"


// Forward declaration
class PostShader;


/**
 * This is a special version of the PostShader that
 * passes through the blur amount.
 */
class PostShaderBlur : public PostShader
{

public:
	PostShaderBlur(std::string shader_file_name);
	void Get_Uniform_Locations();
	void Set_Uniform_Values();
	void Specify_Vertex_Layout();
	void Setup();
	void Cleanup();
	bool Should_Apply();

};


#endif