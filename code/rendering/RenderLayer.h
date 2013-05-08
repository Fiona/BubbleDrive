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
class PostShader;


/**
 * RenderLayers are full screen textures where the
 * Entities draw to in Z order. RenderLayers have post processing
 * effects added to them before being all drawn together.
 */
class RenderLayer
{

public:
	RenderLayer(Shader* primary_shader);
	void Add_Post_Processer_Shader(PostShader* post_shader);
	void Add_Cumilative_Post_Processer_Shader(PostShader* post_shader);
	void Set_As_Active();
	void Set_Texture_As_Active();
	void Unbind();
	void Unbind_Texture();
	void Enable_Primary_Shader();
	void Disable_Primary_Shader();
	void Specify_Vertex_Layout();
	void Do_Post_Processing();
	void Do_Post_Processing_Flip_And_Draw(PostShader* post_effect, int pass);
	bool Do_Cumilative_Post_Processing(std::vector<GLuint>* textures, std::vector<GLuint>* frame_buffers, int* current_fbo);
	void Do_Cumilative_Post_Processing_Flip_And_Draw(PostShader* post_effect, int pass, std::vector<GLuint>* textures, std::vector<GLuint>* frame_buffers, int* current_fbo);

private:
	Game* oGame;
	Shader* oPrimary_Shader;
	std::vector<PostShader*> aPost_Processing_Shaders;
	std::vector<PostShader*> aCumilative_Post_Processing_Shaders;
	std::vector<GLuint> aTexture_Num;
	std::vector<GLuint> aFrame_Buffer_Num;
	int iCurrent_FBO;

};

#endif
