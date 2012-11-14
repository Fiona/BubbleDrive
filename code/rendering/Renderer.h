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
#include "../core/Game.h"
#include "BatchManager.h"
#include "Shader.h"
#include "RenderLayer.h"


// Forward declaration
class RenderLayer;
class BatchManager;


/**
 * Core Renderer class. It has one core method (Render) that the game object
 * calls once per-frame. It tells the batch manager to handle it's batch objects
 * and tells each render layer to draw, do their post processing and then renders
 * it all out to the screen.
 */
class Renderer
{

public:
	Renderer();
	~Renderer();
	void Create_Screen_Sized_FBO(GLuint* texture_num, GLuint* frame_buffer_num);
	void Render();
	void Set_Current_Render_Layer(int render_layer);
	void Unbind_Render_Layer(int render_layer);
	void Specify_Vertex_Layout_For_Render_Layer(int render_layer);

	BatchManager* oBatch_Manager;
	int iCurrent_Render_Layer;
	GLuint oQuad_VAO;
	GLuint oQuad_VBO;

private:
	Game* oGame;
	std::vector<int> aRender_Layer_Order;
	std::vector<GLuint> aScreen_Texture_Num;
	std::vector<GLuint> aScreen_Frame_Buffer_Num;
	std::map<int, Shader*> oShaders;
	std::map<int, RenderLayer*> oRender_Layers;
	int iCurrent_FBO;

};


#endif