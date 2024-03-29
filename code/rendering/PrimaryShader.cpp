/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

/**
   PrimaryShader object source file.
 */


// Includes
#include "PrimaryShader.h"


/**
 * Constructor, pretty much just passes on to the parent.
 */
PrimaryShader::PrimaryShader(std::string shader_file_name, bool is_screen_shader) : Shader(shader_file_name)
{
	bIs_Screen_Shader = is_screen_shader;
	oAttribute_Pos_Rotation_Scale = -1;
	oAttribute_Vertex_Coord = -1;
	oAttribute_Vertex_Colour = -1;
	oAttribute_Texture_Coord = -1;
	Get_Uniform_Locations();
}


/**
 * Gets locations of uniforms for shader.
 */
void PrimaryShader::Get_Uniform_Locations()
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

	if(!bIs_Screen_Shader)
	{
		oUniforms.insert(
			std::pair<std::string, GLint>(
				"camera_position",
				glGetUniformLocation(oShader_Program, "camera_position")
				)
			);
	}

	oUniforms.insert(
		std::pair<std::string, GLint>(
			"screen_size",
			glGetUniformLocation(oShader_Program, "screen_size")
			)
		);

	glUniform1i(oUniforms["texture_num"], 0);
	glUniform1i(oUniforms["screen_texture_num"], 0);
	float screen_size[2]; screen_size[0] = DEFAULT_SCREEN_WIDTH; screen_size[1] = DEFAULT_SCREEN_HEIGHT;
	glUniform2fv(oUniforms["screen_size"], 1, screen_size);

	oAttribute_Pos_Rotation_Scale = glGetAttribLocation(oShader_Program, "pos_rot_scale");
	oAttribute_Vertex_Coord = glGetAttribLocation(oShader_Program, "vertex_coord");
	oAttribute_Vertex_Colour = glGetAttribLocation(oShader_Program, "vertex_colour");
	oAttribute_Texture_Coord = glGetAttribLocation(oShader_Program, "texture_coord");

	glUseProgram(0);

}


/**
 * Used by the render set up to set uniform vals.
 */
void PrimaryShader::Set_Uniform_Values()
{

	if(!bIs_Screen_Shader)
		glUniform4fv(oUniforms["camera_position"], 1, oGame->aCamera_Position);

}


/**
 * Does everything that is required before rendering.
 */
void PrimaryShader::Setup()
{

	glUseProgram(oShader_Program);
	Set_Uniform_Values();

}


/**
 * This is called on the relevant layer after each VBO for a Batch is created.
 */
void PrimaryShader::Specify_Vertex_Layout()
{

	glEnableVertexAttribArray(oAttribute_Vertex_Coord);
	glVertexAttribPointer(oAttribute_Vertex_Coord, 2, GL_FLOAT, GL_FALSE, NUM_ELEMENTS_PER_VERTEX * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(oAttribute_Vertex_Colour);
	glVertexAttribPointer(oAttribute_Vertex_Colour, 4, GL_FLOAT, GL_FALSE, NUM_ELEMENTS_PER_VERTEX * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(oAttribute_Texture_Coord);
	glVertexAttribPointer(oAttribute_Texture_Coord, 2, GL_FLOAT, GL_FALSE, NUM_ELEMENTS_PER_VERTEX * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(oAttribute_Pos_Rotation_Scale);
	glVertexAttribPointer(oAttribute_Pos_Rotation_Scale, 4, GL_FLOAT, GL_FALSE, NUM_ELEMENTS_PER_VERTEX * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));

}


/**
 * Cleans up, disables GL modes etc
 */
void PrimaryShader::Cleanup()
{

	glUseProgram(0);

}