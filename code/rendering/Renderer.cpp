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
#include "PostShader.h"
#include "PostShaderBlur.h"
#include "PostShaderLights.h"
#include "PostShaderSaturation.h"
#include "PostShaderRipple.h"


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
	// These are used to render vertex batches to or to render post-processing passes to.
	// Two are required so render passes have access to what is currently drawn to the screen to
	// that point. The active buffer is switched between passes like a double-buffer.
	iCurrent_FBO = 0;
	aScreen_Texture_Num.resize(2, 0);
	aScreen_Frame_Buffer_Num.resize(2, 0);
	Create_Screen_Sized_FBO(&aScreen_Texture_Num[0], &aScreen_Frame_Buffer_Num[0]);
	Create_Screen_Sized_FBO(&aScreen_Texture_Num[1], &aScreen_Frame_Buffer_Num[1]);
	
	// Create shader objects
	// These load GLSL code and are responsible for passing uniforms etc to the shaders when rendering.
	// PrimaryShaders are used for rendering entity verticies to render layers. Different GLSL code is used for the nature of 
	// the entities. When rendering all entities are drawn to each individual layer first, each layer having it's own FBO.
	// RenderLayerShader is a simple shader that draws a screen-size quad to the screen when collating render layers
	// together for the final image.
	// The Post shaders are used for various post-processing effects. They can be applied to individual layers or to the whole 
	// screen as certain layers are collated.
	oShaders.insert(std::pair<int, Shader*>(SHADER_PRIMARY_SCREEN, new PrimaryShader("screen", true)));
	oShaders.insert(std::pair<int, Shader*>(SHADER_PRIMARY_WORLD, new PrimaryShader("world", false)));
	oShaders.insert(std::pair<int, Shader*>(SHADER_RENDER_LAYER, new RenderLayerShader("renderlayer")));
	oShaders.insert(std::pair<int, Shader*>(SHADER_POST_GREYSCALE, new PostShaderSaturation("greyscale")));
	oShaders.insert(std::pair<int, Shader*>(SHADER_POST_BLUR, new PostShaderBlur("blur")));
	oShaders.insert(std::pair<int, Shader*>(SHADER_POST_BLUR2, new PostShaderBlur("blur2")));
	oShaders.insert(std::pair<int, Shader*>(SHADER_POST_LIGHTS, new PostShaderLights("lights")));
	oShaders.insert(std::pair<int, Shader*>(SHADER_POST_RIPPLE, new PostShaderRipple("ripple")));

	// Create render layers
	// These are essentially FBOs that entity batches are drawn to. Different layers are used so certain post-processing
	// effects will only apply to certain screen layers.
	oRender_Layers.insert(
		std::pair<int, RenderLayer*>(
			RENDER_LAYER_WORLD_LIT,
			new RenderLayer(oShaders[SHADER_PRIMARY_WORLD])
			)
		);
	
	oRender_Layers.insert(
		std::pair<int, RenderLayer*>(
			RENDER_LAYER_WORLD,
			new RenderLayer(oShaders[SHADER_PRIMARY_WORLD])
			)
		);

	oRender_Layers.insert(
		std::pair<int, RenderLayer*>(
			RENDER_LAYER_SCREEN,
			new RenderLayer(oShaders[SHADER_PRIMARY_SCREEN])
			)
		);
	
	// Add basic per-layer post processing to each render layer where appropriate
	//oRender_Layers[RENDER_LAYER_WORLD]->Add_Post_Processer_Shader(dynamic_cast<PostShader*>(oShaders[SHADER_POST_GREYSCALE]));
	//oRender_Layers[RENDER_LAYER_WORLD_LIT]->Add_Post_Processer_Shader(dynamic_cast<PostShader*>(oShaders[SHADER_POST_BLUR]));
	//oRender_Layers[RENDER_LAYER_WORLD_LIT]->Add_Post_Processer_Shader(dynamic_cast<PostShader*>(oShaders[SHADER_POST_BLUR2]));
	oRender_Layers[RENDER_LAYER_WORLD_LIT]->Add_Post_Processer_Shader(dynamic_cast<PostShader*>(oShaders[SHADER_POST_LIGHTS]));
	
	// Cumilative post-processing shaders are be applied to layers as they are added to the final screen buffer. This allows us to have
	// a shader that affects everything drawn up to a point. For instance, affecting everything execpt on-screen GUI.
	oRender_Layers[RENDER_LAYER_WORLD_LIT]->Add_Cumilative_Post_Processer_Shader(dynamic_cast<PostShader*>(oShaders[SHADER_POST_BLUR]));
	oRender_Layers[RENDER_LAYER_WORLD_LIT]->Add_Cumilative_Post_Processer_Shader(dynamic_cast<PostShader*>(oShaders[SHADER_POST_BLUR2]));
	//oRender_Layers[RENDER_LAYER_WORLD_LIT]->Add_Cumilative_Post_Processer_Shader(dynamic_cast<PostShader*>(oShaders[SHADER_POST_LIGHTS]));
	oRender_Layers[RENDER_LAYER_WORLD_LIT]->Add_Cumilative_Post_Processer_Shader(dynamic_cast<PostShader*>(oShaders[SHADER_POST_GREYSCALE]));
	oRender_Layers[RENDER_LAYER_WORLD_LIT]->Add_Cumilative_Post_Processer_Shader(dynamic_cast<PostShader*>(oShaders[SHADER_POST_RIPPLE]));

	// Define what order we draw layers in from back to front
	aRender_Layer_Order.push_back(RENDER_LAYER_WORLD);
	aRender_Layer_Order.push_back(RENDER_LAYER_WORLD_LIT);
	aRender_Layer_Order.push_back(RENDER_LAYER_SCREEN);

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

	// Recreating the view frustrum that our shaders use in code for use various
	// screen<->world operations.
	oView_Frustrum = boost::numeric::ublas::matrix<float>(4, 4);
	float aspect_ratio = (float)DEFAULT_SCREEN_WIDTH / (float)DEFAULT_SCREEN_HEIGHT;
	float angle_of_view = oGame->Deg_To_Rad(ANGLE_OF_VIEW);
	float z_near = 0.5f;
	float z_far = 3001.0f;

	// GLSL matricies are defined as column,row 
	oView_Frustrum(0,0) = 1.0f / (float)tan(angle_of_view); oView_Frustrum(1,0) = 0.0f; oView_Frustrum(2,0) = 0.0f; oView_Frustrum(3,0) = 0.0f;
	oView_Frustrum(0,1) = 0.0f; oView_Frustrum(1,1) = (-aspect_ratio) / (float)tan(angle_of_view); oView_Frustrum(2,1) = 0.0f; oView_Frustrum(3,1) = 0.0f;
	oView_Frustrum(0,2) = 0.0f; oView_Frustrum(1,2) = 0.0f; oView_Frustrum(2,2) = (z_far+z_near)/(z_far-z_near); oView_Frustrum(3,2) = 1.0f;
	oView_Frustrum(0,3) = 0.0f; oView_Frustrum(1,3) = 0.0f; oView_Frustrum(2,3) = -2.0f * z_far * z_near / (z_far - z_near); oView_Frustrum(3,3) = 0.0f;

	a00 = oView_Frustrum(0,0); a10 = oView_Frustrum(1,0); a20 = oView_Frustrum(2,0); a30 = oView_Frustrum(3,0);
	a01 = oView_Frustrum(0,1); a11 = oView_Frustrum(1,1); a21 = oView_Frustrum(2,1); a31 = oView_Frustrum(3,1);
	a02 = oView_Frustrum(0,2); a12 = oView_Frustrum(1,2); a22 = oView_Frustrum(2,2); a32 = oView_Frustrum(3,2);
	a03 = oView_Frustrum(0,3); a13 = oView_Frustrum(1,3); a23 = oView_Frustrum(2,3); a33 = oView_Frustrum(3,3);

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

	while(!aLights.empty()) delete aLights.back(), aLights.pop_back();
	aLights.clear();

	while(!aShockwaves.empty()) delete aShockwaves.back(), aShockwaves.pop_back();
	aShockwaves.clear();

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
    for(std::vector<int>::iterator it = aRender_Layer_Order.begin(); it != aRender_Layer_Order.end(); ++it)
    {		
        oRender_Layers[*it]->Set_As_Active();
	    glClear(GL_COLOR_BUFFER_BIT);
    }
	
	// When rendering to the frame buffers we need to make sure we're not
	// taking the destination colour into account otherwise we will blend with the
	// transparent black pixels that it's cleared too. Yeah, I know, right?
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	// Tell BatchManager to Update and draw
	// batches to their RenderLayer textures
	oBatch_Manager->Update_And_Render_Batches();

	// We have to put the blend function back to something a little more sane
	// for collating the FBOs together.
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	// Now we have drawn each entity to their relevant RenderLayer
	// and their frame buffer objects. We interate through 
	// each RenderLayer and apply post-processing shaders while
	// drawing each one to a temporary buffer that will finally
	// be rendered to the screen.

	// Bind a single quad, this will be our "fake" screen
	glBindVertexArray(oQuad_VAO);

	// Make sure all the post processing that needs doing to individual render layers happens
    for(std::vector<int>::iterator it = aRender_Layer_Order.begin(); it != aRender_Layer_Order.end(); ++it)
        oRender_Layers[*it]->Do_Post_Processing();

	// Bind the temporary draw buffer and clear the drawing that was
	// done on it the previous frame.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, aScreen_Frame_Buffer_Num[iCurrent_FBO]);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Setup the default shader for simple full-screen single-quad drawing.
	oShaders[SHADER_RENDER_LAYER]->Setup();

	// With each RenderLayer, draw them as a screen-sized quad
    for(std::vector<int>::iterator it = aRender_Layer_Order.begin(); it != aRender_Layer_Order.end(); ++it)
    {

		oRender_Layers[*it]->Set_Texture_As_Active();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// One each one is drawn we make sure that we do any post processing that needs doing to it.
		if(oRender_Layers[*it]->Do_Cumilative_Post_Processing(&aScreen_Texture_Num, &aScreen_Frame_Buffer_Num, &iCurrent_FBO))
			oShaders[SHADER_RENDER_LAYER]->Setup();

    }

	// Draw collated texture to the screen
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, aScreen_Texture_Num[iCurrent_FBO]);
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


/**
 * Essentially just adds a Light object to the Renderer's list of lights.
 * These objects are used by PostShaderLights to do lighting effects.
 */
void Renderer::Register_Light(Light* new_light)
{

	aLights.push_back(new_light);

}


/**
 * This removes Light objects from the light list.
 */
void Renderer::Remove_Light(Light* light)
{
    std::vector<Light*>::iterator it;
    it = std::find(aLights.begin(), aLights.end(), light);
    if(it != aLights.end())
	{
		delete(*it);
        it = aLights.erase(it);
	}
}


/**
 * Adds shockwave ripple objects, similar to lights.
 * These objects are used by PostShaderRipple.
 */
void Renderer::Register_Shockwave(Shockwave* new_shockwave)
{

	aShockwaves.push_back(new_shockwave);

}


/**
 * This removes Shockwave objects from the shockwave list.
 */
void Renderer::Remove_Shockwave(Shockwave* shockwave)
{
    std::vector<Shockwave*>::iterator it;
    it = std::find(aShockwaves.begin(), aShockwaves.end(), shockwave);
    if(it != aShockwaves.end())
	{
		delete(*it);
        it = aShockwaves.erase(it);
	}
}
