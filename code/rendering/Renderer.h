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
#include <boost/numeric/ublas/matrix.hpp>
#include "../core/Game.h"
#include "BatchManager.h"
#include "Shader.h"
#include "RenderLayer.h"
#include "Light.h"
#include "Shockwave.h"


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
	void Register_Light(Light* new_light);
	void Remove_Light(Light* light);
	void Register_Shockwave(Shockwave* new_shockwave);
	void Remove_Shockwave(Shockwave* shockwave);

	BatchManager* oBatch_Manager;
	int iCurrent_Render_Layer;
	GLuint oQuad_VAO;
	GLuint oQuad_VBO;
	boost::numeric::ublas::matrix<float> oView_Frustrum;

	float a00; float a10; float a20; float a30;
	float a01; float a11; float a21; float a31;
	float a02; float a12; float a22; float a32;
	float a03; float a13; float a23; float a33;

	std::vector<Light*> aLights;
	std::vector<Shockwave*> aShockwaves;

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
