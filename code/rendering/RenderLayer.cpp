/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

/**
   Source for render layers
 */


// Includes
#include "RenderLayer.h"
#include "PostShader.h"


/**
 * Constructor
 */
RenderLayer::RenderLayer(Shader* primary_shader)
{
	oPrimary_Shader = primary_shader;
	oGame = Game::Instance();
	aTexture_Num.resize(2, 0);
	aFrame_Buffer_Num.resize(2, 0);
	iCurrent_FBO = 0;
	oGame->oRenderer->Create_Screen_Sized_FBO(&aTexture_Num[0], &aFrame_Buffer_Num[0]);
	oGame->oRenderer->Create_Screen_Sized_FBO(&aTexture_Num[1], &aFrame_Buffer_Num[1]);
}


/**
 * Adds a shader that is applied to the rendering layer after
 * all entities are drawn to it.
 */
void RenderLayer::Add_Post_Processer_Shader(PostShader* post_shader)
{
	aPost_Processing_Shaders.push_back(post_shader);
}


/**
 * Sets the render layer so it's the one that will be drawn to.
 */
void RenderLayer::Set_As_Active()
{
	glBindFramebuffer(GL_FRAMEBUFFER, aFrame_Buffer_Num[iCurrent_FBO]);
}


/**
 * Sets the FBO texture as the active texture
 */
void RenderLayer::Set_Texture_As_Active()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, aTexture_Num[iCurrent_FBO]);
}

/**
 *
 */
void RenderLayer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
*
 */
void RenderLayer::Unbind_Texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


/**
 * Called to make sure that the primary shader is used.
 */
void RenderLayer::Enable_Primary_Shader()
{
	oPrimary_Shader->Setup();
}


/**
 * 
 */
void RenderLayer::Disable_Primary_Shader()
{
	oPrimary_Shader->Cleanup();
}


/**
 * Passes on to the primary shader.
 */
void RenderLayer::Specify_Vertex_Layout()
{
	oPrimary_Shader->Specify_Vertex_Layout();
}


/**
 * Once things have been drawn to the render layer, this will run
 * post processing shaders that have been assigned to this layer.
 */
void RenderLayer::Do_Post_Processing()
{

	if(aPost_Processing_Shaders.size() == 0)
		return;
	
    for(std::vector<PostShader*>::iterator it = aPost_Processing_Shaders.begin(); it != aPost_Processing_Shaders.end(); ++it)
	{

		if(!(*it)->Should_Apply())
			continue;

		// We use a kind of double buffered thing for our FBO so 
		// we're not writing to the same thing we're reading from, 
		// flipping between FBOs when we get here.
		glBindTexture(GL_TEXTURE_2D, aTexture_Num[iCurrent_FBO]);

		if(iCurrent_FBO == 0)
			iCurrent_FBO = 1;
		else
			iCurrent_FBO = 0;

		glBindFramebuffer(GL_FRAMEBUFFER, aFrame_Buffer_Num[iCurrent_FBO]);
		glClear(GL_COLOR_BUFFER_BIT);

		(*it)->Setup();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		(*it)->Cleanup();

	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}