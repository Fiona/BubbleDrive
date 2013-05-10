/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************/

#pragma once
#ifndef _POST_SHADER_SATURATION_H_
#define _POST_SHADER_SATURATION_H_

/**
 * PostShaderSaturation object header.
 */


// Includes
#include <string>
#include "PostShader.h"


// Forward declaration
class PostShader;


/**
 * This is a special version of the PostShader that
 * adjusts the overall colour saturation.
 */
class PostShaderSaturation : public PostShader
{

public:
	PostShaderSaturation(std::string shader_file_name);
	void Get_Uniform_Locations();
	void Set_Uniform_Values(int pass);
	void Specify_Vertex_Layout();
	void Setup(int pass);
	void Cleanup();
	bool Should_Apply();

};


#endif