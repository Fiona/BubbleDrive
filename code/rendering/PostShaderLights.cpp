/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Nov 2012 by Fiona ****
 ***************************************/

/**
   PostShaderLights object source file.
 */


// Includes
#include "PostShaderLights.h"


/**
 * Constructor, pretty much just passes on to the parent.
 */
PostShaderLights::PostShaderLights(std::string shader_file_name) : PostShader(shader_file_name)
{
	Get_Uniform_Locations();
}


/**
 * Gets locations of uniforms for shader.
 */
void PostShaderLights::Get_Uniform_Locations()
{

	PostShader::Get_Uniform_Locations();

	glUseProgram(oShader_Program);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"camera_position",
			glGetUniformLocation(oShader_Program, "camera_position")
			)
		);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"camera_zoom",
			glGetUniformLocation(oShader_Program, "camera_zoom")
			)
		);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"light_coordinate",
			glGetUniformLocation(oShader_Program, "light_coordinate")
			)
		);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"light_size",
			glGetUniformLocation(oShader_Program, "light_size")
			)
		);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"light_colour",
			glGetUniformLocation(oShader_Program, "light_colour")
			)
		);
	glUseProgram(0);

}


/**
 * Used by the render set up to set uniform vals.
 */
void PostShaderLights::Set_Uniform_Values(int pass)
{
	glUniform4fv(oUniforms["camera_position"], 1, oGame->aCamera_Position);
	glUniform1f(oUniforms["camera_zoom"], (oGame->aCamera_Position[2] - (float)(MIN_ZOOM)) / ((float)MAX_ZOOM - (float)(MIN_ZOOM)));

	std::vector<float> light_pos = oGame->World_To_Real_Screen(oGame->oRenderer->aLights[pass-1]->Get_X(), oGame->oRenderer->aLights[pass-1]->Get_Y());
	glUniform2fv(oUniforms["light_coordinate"], 1, &light_pos[0]);

	glUniform1f(oUniforms["light_size"], oGame->World_Length_To_Real_Screen((float)oGame->oRenderer->aLights[pass-1]->Get_Size()));

	std::vector<float> light_colour = oGame->oRenderer->aLights[pass-1]->Get_Colour();
	light_colour.push_back(oGame->oRenderer->aLights[pass-1]->Get_Alpha());
	glUniform4fv(oUniforms["light_colour"], 1, &light_colour[0]);

}


/**
 * Returns a bool on if the shader should be applied or not.
 * This allows us to skip it if it's unnecessary.
 */
bool PostShaderLights::Should_Apply()
{

	return true;

}


/**
 * Returns an int specifying how many times we should apply this
 * post-processing effect. This is usually 1.
 */
int PostShaderLights::Num_Passes_Required()
{

	return oGame->oRenderer->aLights.size();

}


/**
 * Virtual definitons
 */
void PostShaderLights::Setup(int pass)
{
	glUseProgram(oShader_Program);
	Set_Uniform_Values(pass);
}
void PostShaderLights::Specify_Vertex_Layout(){ PostShader::Specify_Vertex_Layout(); }
void PostShaderLights::Cleanup(){ PostShader::Cleanup(); }
