/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _RENDER_LAYER_H_
#define _RENDER_LAYER_H_

/**
 * RenderLayer object header.
 */


// Includes
#include "../core/Game.h"
#include "Shader.h"

// Forward declaration
class Shader;


/**
 * RenderLayers are full screen textures where the
 * Entities draw to in Z order. RenderLayers have post processing
 * effects added to them before being all drawn together.
 */
class RenderLayer
{

public:
	RenderLayer(Shader* primary_shader);
	void Add_Post_Processer_Shader(Shader* post_shader);
	void Set_As_Active();
	void Set_Texture_As_Active(GLuint texture_num = GL_TEXTURE0);
	void Unbind();
	void Unbind_Texture(GLuint texture_num = GL_TEXTURE0);
	void Enable_Primary_Shader();
	void Disable_Primary_Shader();
	void Specify_Vertex_Layout();

private:
	Game* oGame;
	Shader* oPrimary_Shader;
	std::vector<Shader*> aPost_Processing_Shaders;
	GLuint iTexture_Num;
	GLuint iFrame_Buffer_Num;

};

#endif
