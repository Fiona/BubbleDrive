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


/**
 * Constructor
 */
RenderLayer::RenderLayer(Shader* primary_shader)
{
	oPrimary_Shader = primary_shader;
	oGame = Game::Instance();
	oGame->oRenderer->Create_Screen_Sized_FBO(&iTexture_Num, &iFrame_Buffer_Num);
}


/**
 * Adds a shader that is applied to the rendering layer after
 * all entities are drawn to it.
 */
void RenderLayer::Add_Post_Processer_Shader(Shader* post_shader)
{
	aPost_Processing_Shaders.push_back(post_shader);
}


/**
 * Sets the render layer so it's the one that will be drawn to.
 */
void RenderLayer::Set_As_Active()
{
	glBindFramebuffer(GL_FRAMEBUFFER, iFrame_Buffer_Num);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, iTexture_Num);
}


/**
 *
 */
void RenderLayer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE1);
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