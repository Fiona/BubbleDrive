/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************/

/**
   PostShaderSaturation object source file.
 */


// Includes
#include "PostShaderSaturation.h"


/**
 * Constructor, pretty much just passes on to the parent.
 */
PostShaderSaturation::PostShaderSaturation(std::string shader_file_name) : PostShader(shader_file_name)
{
	Get_Uniform_Locations();
}


/**
 * Gets locations of uniforms for shader.
 */
void PostShaderSaturation::Get_Uniform_Locations()
{

	PostShader::Get_Uniform_Locations();

	glUseProgram(oShader_Program);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"saturation_amount",
			glGetUniformLocation(oShader_Program, "saturation_amount")
			)
		);
	glUseProgram(0);

}


/**
 * Used by the render set up to set uniform vals.
 */
void PostShaderSaturation::Set_Uniform_Values(int pass)
{
	glUniform1f(oUniforms["saturation_amount"], oGame->fSaturation_Amount);
	//glUniform1f(oUniforms["saturation_amount"], 0.0f);
}


/**
 * Returns a bool on if the shader should be applied or not.
 * This allows us to skip it if it's unnecessary.
 */
bool PostShaderSaturation::Should_Apply()
{

	return true;
	//return (oGame->fSaturation_Amount != 1.0f ? true : false);

}


/**
 * Virtual definitons
 */
void PostShaderSaturation::Setup(int pass){ PostShader::Setup(pass); }
void PostShaderSaturation::Specify_Vertex_Layout(){ PostShader::Specify_Vertex_Layout(); }
void PostShaderSaturation::Cleanup(){ PostShader::Cleanup(); }
