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
#include "Shader.h"

// Forward declaration
class Shader;


/**
 * RenderLayers are full screen textures where the
 * Entities draw to in Z order. RenderLayers have post processing
 * effects added to them before being all drawn together.
 */
class RenderLayer
{

public:
	RenderLayer(Shader* primary_shader);
	void Add_Post_Processer_Shader(Shader* post_shader);

private:
	Shader* oPrimary_Shader;
	std::vector<Shader*> aPost_Processing_Shaders;

};

#endif
