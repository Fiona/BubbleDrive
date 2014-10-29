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
#include "../core/Media.h"
#include "PostShaderRipple.h"
#include "Renderer.h"


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
			"shockwave_texture_num",
			glGetUniformLocation(oShader_Program, "shockwave_texture_num")
			)
		);

	oUniforms.insert(
		std::pair<std::string, GLint>(
			"shockwave_coordinate",
			glGetUniformLocation(oShader_Program, "shockwave_coordinate")
			)
		);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"shockwave_size",
			glGetUniformLocation(oShader_Program, "shockwave_size")
			)
		);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"shockwave_alpha",
			glGetUniformLocation(oShader_Program, "shockwave_alpha")
			)
		);

	glUniform1i(oUniforms["shockwave_texture_num"], 2);
	glUseProgram(0);

}


/**
 * Used by the render set up to set uniform vals.
 */
void PostShaderRipple::Set_Uniform_Values(int pass)
{

	glUniform1i(oUniforms["shockwave_texture_num"], 2);

    if(Num_Passes_Required() == 0)
    {
        glUniform1f(oUniforms["shockwave_size"], 0);
        return; 
    }

	std::vector<float> shock_pos = oGame->World_To_Real_Screen(oGame->oRenderer->aShockwaves[pass-1]->Get_X(), oGame->oRenderer->aShockwaves[pass-1]->Get_Y());
	glUniform2fv(oUniforms["shockwave_coordinate"], 1, &shock_pos[0]);

	glUniform1f(oUniforms["shockwave_size"], oGame->World_Length_To_Real_Screen((float)oGame->oRenderer->aShockwaves[pass-1]->Get_Size()));
	glUniform1f(oUniforms["shockwave_alpha"], oGame->oRenderer->aShockwaves[pass-1]->Get_Alpha());

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
 * Returns an int specifying how many times we should apply this
 * post-processing effect. This is usually 1.
 */
int PostShaderRipple::Num_Passes_Required()
{

	return oGame->oRenderer->aShockwaves.size();

}

/**
 * Virtual definitons
 */
void PostShaderRipple::Setup(int pass)
{

    // We have to bind the shockwave image to the default framebuffer, so we find out what the
    // active one is and rebind it after binding the texture to texture 2
    GLint current_fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, oGame->oMedia->mImages["shader_shockwave"]->iTexture_Num);

	glBindFramebuffer(GL_FRAMEBUFFER, current_fbo);
    glActiveTexture(GL_TEXTURE0);

    PostShader::Setup(pass);

}

void PostShaderRipple::Specify_Vertex_Layout(){ PostShader::Specify_Vertex_Layout(); }
void PostShaderRipple::Cleanup(){ PostShader::Cleanup(); }
