/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _RENDERER_H_
#define _RENDERER_H_


// Includes
#include <map>
#include <GL/glew.h>
#include "BatchManager.h"
#include "Shader.h"
#include "PrimaryShader.h"
#include "RenderLayer.h"


class Renderer
{

public:
	Renderer();
	~Renderer();
	void Create_Screen_Sized_FBO(GLuint* texture_num, GLuint* frame_buffer_num);
	void Render();

	BatchManager* oBatch_Manager;
	GLuint iFullscreen_FBO_Verticies;

private:
	GLuint iScreen_Texture_Num;
	GLuint iScreen_Frame_Buffer_Num;
	std::map<int, Shader*> oShaders;
	std::map<int, RenderLayer*> oRender_Layers;

};


#endif