/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Nov 2012 by Fiona ****
 ***************************************/

/**
   PostShader object source file.
 */


// Includes
#include "PostShader.h"


/**
 * Constructor, pretty much just passes on to the parent.
 */
PostShader::PostShader(std::string shader_file_name) : Shader(shader_file_name)
{
	Get_Uniform_Locations();
}


/**
 * Gets locations of uniforms for shader.
 */
void PostShader::Get_Uniform_Locations()
{

	glUseProgram(oShader_Program);

	oUniforms.insert(
		std::pair<std::string, GLint>(
			"texture_num",
			glGetUniformLocation(oShader_Program, "texture_num")
			)
		);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"screen_texture_num",
			glGetUniformLocation(oShader_Program, "screen_texture_num")
			)
		);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"screen_size",
			glGetUniformLocation(oShader_Program, "screen_size")
			)
		);

	glUniform1i(oUniforms["texture_num"], 0);
	glUniform1i(oUniforms["screen_texture_num"], 1);
	float screen_size[2]; screen_size[0] = DEFAULT_SCREEN_WIDTH; screen_size[1] = DEFAULT_SCREEN_HEIGHT;
	glUniform2fv(oUniforms["screen_size"], 1, screen_size);

	oAttribute_Vertex_Coord = glGetAttribLocation(oShader_Program, "vertex_coord");
	oAttribute_Texture_Coord = glGetAttribLocation(oShader_Program, "texture_coord");
	glEnableVertexAttribArray(oAttribute_Vertex_Coord);
	glEnableVertexAttribArray(oAttribute_Texture_Coord);

	glUseProgram(0);

}


/**
 * Used by the render set up to set uniform vals.
 */
void PostShader::Set_Uniform_Values()
{

}


/**
 * Does everything that is required before rendering.
 */
void PostShader::Setup()
{

	glUseProgram(oShader_Program);
	Set_Uniform_Values();

}


/**
 * This is called on the relevant layer after each VBO for a Batch is created.
 */
void PostShader::Specify_Vertex_Layout()
{

	glEnableVertexAttribArray(oAttribute_Vertex_Coord);
	glVertexAttribPointer(oAttribute_Vertex_Coord, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(oAttribute_Texture_Coord);
	glVertexAttribPointer(oAttribute_Texture_Coord, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

}


/**
 * Cleans up, disables GL modes etc
 */
void PostShader::Cleanup()
{

	glUseProgram(0);

}


/**
 * Returns a bool on if the shader should be applied or not.
 * This allows us to skip it if it's unnecessary.
 */
bool PostShader::Should_Apply()
{
	return true;
}

