/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

/**
 * Renderer object source file.
 */

// Includes
#include "../core/consts.h"
#include "Renderer.h"
#include "PrimaryShader.h"


/**
 * Constructor. Creates BatchManager, Shaders and 
 * RenderLayers.
 */
Renderer::Renderer()
{

	oGame = Game::Instance();
	iCurrent_Render_Layer = -1;

	oBatch_Manager = new BatchManager();

	// Create screen sized texture and frame buffer
	Create_Screen_Sized_FBO(&iScreen_Texture_Num, &iScreen_Frame_Buffer_Num);

	// Create shader objects
	oShaders.insert(std::pair<int, Shader*>(SHADER_PRIMARY_SCREEN, new PrimaryShader("screen", true)));
	oShaders.insert(std::pair<int, Shader*>(SHADER_PRIMARY_WORLD, new PrimaryShader("world", false)));

	// Create render layers
	oRender_Layers.insert(
		std::pair<int, RenderLayer*>(
			RENDER_LAYER_WORLD,
			new RenderLayer(oShaders[SHADER_PRIMARY_WORLD])
			)
		);

	oRender_Layers.insert(
		std::pair<int, RenderLayer*>(
			RENDER_LAYER_WORLD_LIT,
			new RenderLayer(oShaders[SHADER_PRIMARY_WORLD])
			)
		);

	oRender_Layers.insert(
		std::pair<int, RenderLayer*>(
			RENDER_LAYER_SCREEN,
			new RenderLayer(oShaders[SHADER_PRIMARY_SCREEN])
			)
		);

	// Create vertex buffer for drawing a full screen texture
	GLfloat fbo_vertices[] = {
		-1, -1,
	     1, -1,
		-1,  1,
	     1,  1,
	};
	glGenBuffers(1, &iFullscreen_FBO_Verticies);
	glBindBuffer(GL_ARRAY_BUFFER, iFullscreen_FBO_Verticies);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}


/**
 * Destructor
 */
Renderer::~Renderer()
{

	delete(oBatch_Manager);
	
    for(std::map<int, Shader* >::iterator it = oShaders.begin(); it != oShaders.end(); ++it)
    {
		if(it->second == NULL)
            continue;
		delete(it->second);
    }

	for(std::map<int, RenderLayer* >::iterator it = oRender_Layers.begin(); it != oRender_Layers.end(); ++it)
    {
		if(it->second == NULL)
            continue;
		delete(it->second);
    }

	glDeleteBuffers(1, &iFullscreen_FBO_Verticies);

}


/**
 * Will create a screen sized texture, used by RenderLayers.
 * You need to pass pointers to integers holding the texture and framebuffer nums.
 */
void Renderer::Create_Screen_Sized_FBO(GLuint* texture_num, GLuint* frame_buffer_num)
{

	// Create texture and fill full of junk data
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, texture_num);
	glBindTexture(GL_TEXTURE_2D, *texture_num);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create FBO to link it all up
	glGenFramebuffers(1, frame_buffer_num);
	glBindFramebuffer(GL_FRAMEBUFFER, *frame_buffer_num);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture_num, 0);

	// Check it worked
	GLenum status;
	if((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
	{
		// fail
		// ... 
		// fprintf(stderr, "glCheckFramebufferStatus: error %p", status);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);

}


/**
 * Core render method. Called by the game every draw frame.
 */
void Renderer::Render()
{

    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

	// Clear RenderLayers
	iCurrent_Render_Layer = -1;
    for(std::map<int, RenderLayer* >::iterator it = oRender_Layers.begin(); it != oRender_Layers.end(); ++it)
    {
        it->second->Set_As_Active();
	    glClear(GL_COLOR_BUFFER_BIT);
	    it->second->Unbind();
    }
	
	// Tell BatchManager to Update and draw
	// batches to their RenderLayer textures
	oBatch_Manager->Update_And_Render_Batches();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);

	if(iCurrent_Render_Layer != -1)
		Unbind_Render_Layer(iCurrent_Render_Layer);

	// Do post-processing and draw each RenderLayer to
	// the screen texture
	// ...

	// Draw screen texture to screen
	// ...

}


/**
 * Called by Batch objects to ensure that the current
 * render layer is the one being used.
 */
void Renderer::Set_Current_Render_Layer(int render_layer)
{

	if(iCurrent_Render_Layer == render_layer)
	{
		//oRender_Layers[render_layer]->Enable_Primary_Shader();
		return;
	}

	if(iCurrent_Render_Layer != -1)
		Unbind_Render_Layer(iCurrent_Render_Layer);

	iCurrent_Render_Layer = render_layer;

	oRender_Layers[iCurrent_Render_Layer]->Set_As_Active();
	oRender_Layers[iCurrent_Render_Layer]->Enable_Primary_Shader();

}


/*
 *
 */
void Renderer::Unbind_Render_Layer(int render_layer)
{

	oRender_Layers[iCurrent_Render_Layer]->Disable_Primary_Shader();
	oRender_Layers[iCurrent_Render_Layer]->Unbind();
	iCurrent_Render_Layer = -1;

}