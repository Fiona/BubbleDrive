/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************/

/**
   PostShaderRipple object source file.
 */


// Includes
#include "PostShaderRipple.h"


/**
 * Constructor, pretty much just passes on to the parent.
 */
PostShaderRipple::PostShaderRipple(std::string shader_file_name) : PostShader(shader_file_name)
{
	Get_Uniform_Locations();
}


/**
 * Gets locations of uniforms for shader.
 */
void PostShaderRipple::Get_Uniform_Locations()
{

	PostShader::Get_Uniform_Locations();

	glUseProgram(oShader_Program);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"game_time",
			glGetUniformLocation(oShader_Program, "game_time")
			)
		);
	glUniform1f(oUniforms["game_time"], 0.0f);
	glUseProgram(0);

}


/**
 * Used by the render set up to set uniform vals.
 */
void PostShaderRipple::Set_Uniform_Values(int pass)
{
	float time = oGame->oGame_Time->getElapsedTime().asSeconds();
	glUniform1f(oUniforms["game_time"], time);
}


/**
 * Returns a bool on if the shader should be applied or not.
 * This allows us to skip it if it's unnecessary.
 */
bool PostShaderRipple::Should_Apply()
{

	return true;

}


/**
 * Virtual definitons
 */
void PostShaderRipple::Setup(int pass){ PostShader::Setup(pass); }
void PostShaderRipple::Specify_Vertex_Layout(){ PostShader::Specify_Vertex_Layout(); }
void PostShaderRipple::Cleanup(){ PostShader::Cleanup(); }
