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
}


/**
 * Adds a shader that is applied to the rendering layer after
 * all entities are drawn to it.
 */
void RenderLayer::Add_Post_Processer_Shader(Shader* post_shader)
{
	aPost_Processing_Shaders.push_back(post_shader);
}

