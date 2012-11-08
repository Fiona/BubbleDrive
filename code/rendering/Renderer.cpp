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
#include "RenderLayerShader.h"


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
	oShaders.insert(std::pair<int, Shader*>(SHADER_RENDER_LAYER, new RenderLayerShader("renderlayer")));

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

	// We create a VAO containing a quad for use by full-screen layer drawing
	glGenVertexArrays( 1, &oQuad_VAO);

	std::vector<GLfloat> vbo_data(24, 0.0);
	vbo_data[0] = -1.0f; vbo_data[1] = 1.0f;	vbo_data[2] = 0.0f; vbo_data[3] = 1.0f;
	vbo_data[4] = 1.0f;  vbo_data[5] = 1.0f;	vbo_data[6] = 1.0f; vbo_data[7] = 1.0f;
	vbo_data[8] = 1.0f;  vbo_data[9] = -1.0f;	vbo_data[10] = 1.0f; vbo_data[11] = 0.0f;

	vbo_data[12] = 1.0f; vbo_data[13] = -1.0f;	vbo_data[14] = 1.0f; vbo_data[15] = 0.0f;
	vbo_data[16] = -1.0f; vbo_data[17] = -1.0f;	vbo_data[18] = 0.0f; vbo_data[19] = 0.0f;
	vbo_data[20] = -1.0f; vbo_data[21] = 1.0f;  vbo_data[22] = 0.0f; vbo_data[23] = 1.0f;
	
	glGenBuffers(1, &oQuad_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, oQuad_VBO);
	glBufferData(GL_ARRAY_BUFFER, vbo_data.size() * sizeof(GLfloat), &vbo_data[0], GL_STATIC_DRAW);

	glBindVertexArray(oQuad_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, oQuad_VBO);
	oShaders[SHADER_RENDER_LAYER]->Specify_Vertex_Layout();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	vbo_data.clear();

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

	glDeleteBuffers(1, &oQuad_VBO);
	glDeleteVertexArrays(1, &oQuad_VAO);

}


/**
 * Will create a screen sized texture, used by RenderLayers.
 * You need to pass pointers to integers holding the texture and framebuffer nums.
 */
void Renderer::Create_Screen_Sized_FBO(GLuint* texture_num, GLuint* frame_buffer_num)
{

	// Create texture and fill full of junk data
	glGenTextures(1, texture_num);
	glBindTexture(GL_TEXTURE_2D, *texture_num);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Create FBO to link it all up
	glGenFramebuffers(1, frame_buffer_num);
	glBindFramebuffer(GL_FRAMEBUFFER, *frame_buffer_num);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture_num, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

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

	// Now we have drawn each entity to their relevant RenderLayer
	// and their frame buffer objects we interate through 
	// each RenderLayer and apply post-processing shaders while
	// drawing each one to the screen texture

	// Bind the screen texture
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, iScreen_Frame_Buffer_Num);
	glClear(GL_COLOR_BUFFER_BIT);

	// Enable drawing a quad to the screen using the default shader
	glBindVertexArray(oQuad_VAO);
	oShaders[SHADER_RENDER_LAYER]->Setup();

	// With each RenderLayer, draw them as a screen-sized quad
    for(std::map<int, RenderLayer* >::iterator it = oRender_Layers.begin(); it != oRender_Layers.end(); ++it)
    {
		it->second->Set_Texture_As_Active();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		it->second->Unbind_Texture();
    }

	// Draw screen texture to screen
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, iScreen_Texture_Num);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Clean clean
	oShaders[SHADER_RENDER_LAYER]->Cleanup();

}


/**
 * Called by Batch objects to ensure that the current
 * render layer is the one being used.
 */
void Renderer::Set_Current_Render_Layer(int render_layer)
{

	if(iCurrent_Render_Layer == render_layer)
		return;

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

	oRender_Layers[render_layer]->Disable_Primary_Shader();
	oRender_Layers[render_layer]->Unbind();
	iCurrent_Render_Layer = -1;

}


/**
 * Each VBO ends up calling this so the program knows what the layout
 * of it's layers verticies look like.
 */
void Renderer::Specify_Vertex_Layout_For_Render_Layer(int render_layer)
{

	oRender_Layers[render_layer]->Specify_Vertex_Layout();

}
