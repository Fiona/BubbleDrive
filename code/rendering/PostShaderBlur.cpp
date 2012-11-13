/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Nov 2012 by Fiona ****
 ***************************************/

/**
   PostShaderBlur object source file.
 */


// Includes
#include "PostShaderBlur.h"


/**
 * Constructor, pretty much just passes on to the parent.
 */
PostShaderBlur::PostShaderBlur(std::string shader_file_name) : PostShader(shader_file_name)
{
	Get_Uniform_Locations();
}


/**
 * Gets locations of uniforms for shader.
 */
void PostShaderBlur::Get_Uniform_Locations()
{

	PostShader::Get_Uniform_Locations();

	glUseProgram(oShader_Program);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"blur_amount",
			glGetUniformLocation(oShader_Program, "blur_amount")
			)
		);
	glUniform1f(oUniforms["blur_amount"], 1.0);
	glUseProgram(0);

}


/**
 * Used by the render set up to set uniform vals.
 */
void PostShaderBlur::Set_Uniform_Values()
{
	glUniform1f(oUniforms["blur_amount"], oGame->fScreen_Blur_Amount);
}


/**
 * Returns a bool on if the shader should be applied or not.
 * This allows us to skip it if it's unnecessary.
 */
bool PostShaderBlur::Should_Apply()
{

	return (oGame->fScreen_Blur_Amount != 0.0f ? true : false);

}


/**
 * Virtual definitons
 */
void PostShaderBlur::Setup(){ PostShader::Setup(); }
void PostShaderBlur::Specify_Vertex_Layout(){ PostShader::Specify_Vertex_Layout(); }
void PostShaderBlur::Cleanup(){ PostShader::Cleanup(); }
