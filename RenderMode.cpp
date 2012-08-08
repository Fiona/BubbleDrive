/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Jul 2012 by Fiona ****
 ***************************************/

/**
   RenderMode object source file.
 */


// Includes
#include "RenderMode.h"
#include "consts.h"


/**
 * Constructor for the RenderMode. The shader filename sans extension must
 * be passed in. For instance, passing "screen" will load "shaders/screen.vert"
 * and "shaders/screen.frag" as the vertex and fragment shaders respectively.
 */
RenderMode::RenderMode(std::string shader_file_name)
{

	oShader_Program = 0;
	oFragment_Shader_Program = 0;
	oVertex_Shader_Program = 0;

	oGame = Game::Instance();

	// Load Vertex shader code
	std::string vert_shader = oGame->Load_From_File(PATH_SHADERS + SEPARATOR + shader_file_name + ".vert");
	const char* vertex_shader_code = vert_shader.c_str();
	if(vertex_shader_code == 0)
	{
		fprintf(stderr, "Error loading vertex shader file.\n");
		return;
	}

    oVertex_Shader_Program = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(oVertex_Shader_Program, 1, (const GLchar**)&vertex_shader_code, 0);
    glCompileShader(oVertex_Shader_Program);

	// Load fragment shader code
	std::string frag_shader = oGame->Load_From_File(PATH_SHADERS + SEPARATOR + shader_file_name + ".frag");
	const char* fragment_shader_code = frag_shader.c_str();	
	if(fragment_shader_code == 0)
	{
		fprintf(stderr, "Error loading fragment shader file.\n");
		return;
	}

    oFragment_Shader_Program = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(oFragment_Shader_Program, 1, (const GLchar**)&fragment_shader_code, 0);
    glCompileShader(oFragment_Shader_Program);

	// Create and link shader program, and get shader uniform locations
    oShader_Program = glCreateProgram();
    glAttachShader(oShader_Program, oVertex_Shader_Program);
    glAttachShader(oShader_Program, oFragment_Shader_Program);
    glLinkProgram(oShader_Program);
	Get_Uniform_Locations();

}


/**
 * Deconstructor.
 */
RenderMode::~RenderMode()
{

	if(oShader_Program > 0)
	{
		glDetachShader(oShader_Program, oFragment_Shader_Program);
	    glDetachShader(oShader_Program, oVertex_Shader_Program);
	    glDeleteShader(oFragment_Shader_Program);
		glDeleteShader(oVertex_Shader_Program);
	    glDeleteProgram(oShader_Program);
	}

}


/**
 * Gets locations of uniforms for shader.
 */
void RenderMode::Get_Uniform_Locations()
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
			"camera_position",
			glGetUniformLocation(oShader_Program, "camera_position")
			)
		);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"screen_size",
			glGetUniformLocation(oShader_Program, "screen_size")
			)
		);

	glUniform1i(oUniforms["texture_num"], 0);
	float screen_size[2]; screen_size[0] = DEFAULT_SCREEN_WIDTH; screen_size[1] = DEFAULT_SCREEN_HEIGHT;
	glUniform2fv(oUniforms["screen_size"], 1, screen_size);

	oAttribute_Vertex_Coord = glGetAttribLocation(oShader_Program, "vertex_coord");
	oAttribute_Vertex_Colour = glGetAttribLocation(oShader_Program, "vertex_colour");
	oAttribute_Texture_Coord = glGetAttribLocation(oShader_Program, "texture_coord");
	oAttribute_Pos_Rotation_Scale = glGetAttribLocation(oShader_Program, "pos_rotation_scale");

	glUseProgram(0);

}


/**
 * Used by the render set up to set uniform vals.
 */
void RenderMode::Set_Uniform_Values()
{

	glUniform4fv(oUniforms["camera_position"], 1, oGame->aCamera_Position);

}


/**
 * Does everything that is required before rendering.
 */
void RenderMode::Setup()
{

	// Binding texture
	/*
    if(oGame->iCurrent_Bound_Texture != entity->Get_Image()->iTexture_Num)
    {
        glBindTexture(GL_TEXTURE_2D, entity->Get_Image()->iTexture_Num);
        oGame->iCurrent_Bound_Texture = entity->Get_Image()->iTexture_Num;
    }
	*/

	glUseProgram(oShader_Program);
	Set_Uniform_Values();

	glVertexAttribPointer(oAttribute_Vertex_Coord, 2, GL_FLOAT, GL_FALSE, NUM_ELEMENTS_PER_VERTEX * sizeof(GLfloat), 0);
	glVertexAttribPointer(oAttribute_Vertex_Colour, 4, GL_FLOAT, GL_FALSE, NUM_ELEMENTS_PER_VERTEX * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glVertexAttribPointer(oAttribute_Texture_Coord, 2, GL_FLOAT, GL_FALSE, NUM_ELEMENTS_PER_VERTEX * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glVertexAttribPointer(oAttribute_Pos_Rotation_Scale, 4, GL_FLOAT, GL_FALSE, NUM_ELEMENTS_PER_VERTEX * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(oAttribute_Vertex_Coord);
	glEnableVertexAttribArray(oAttribute_Vertex_Colour);
	glEnableVertexAttribArray(oAttribute_Texture_Coord);
	glEnableVertexAttribArray(oAttribute_Pos_Rotation_Scale);

}


/**
 * Cleans up, disables GL modes etc
 */
void RenderMode::Cleanup()
{

	glDisableVertexAttribArray(oAttribute_Vertex_Colour);
	glDisableVertexAttribArray(oAttribute_Vertex_Coord);
	glDisableVertexAttribArray(oAttribute_Texture_Coord);
	glDisableVertexAttribArray(oAttribute_Pos_Rotation_Scale);

	glUseProgram(0);

}
