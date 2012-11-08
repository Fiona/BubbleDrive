/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

/**
   Shader object source file.
 */


// Includes
#include "Shader.h"


/**
 * Constructor for the Shaders. The shader filename sans extension must
 * be passed in. For instance, passing "screen" will load "resources/shaders/screen.vert"
 * and "resources/shaders/screen.frag" as the vertex and fragment shaders respectively.
 */
Shader::Shader(std::string shader_file_name)
{

	oShader_Program = 0;
	oFragment_Shader_Program = 0;
	oVertex_Shader_Program = 0;

	oGame = Game::Instance();

	// Load Vertex shader code
	std::string vert_shader = oGame->Load_From_File(
		PATH_RESOURCES + SEPARATOR + PATH_SHADERS + SEPARATOR + shader_file_name + ".vert"
		);
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
	std::string frag_shader = oGame->Load_From_File(
		PATH_RESOURCES + SEPARATOR + PATH_SHADERS + SEPARATOR + shader_file_name + ".frag"
		);
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
Shader::~Shader()
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
 * Default virtual methods
 */
void Shader::Get_Uniform_Locations(){}
void Shader::Set_Uniform_Values(){}
void Shader::Specify_Vertex_Layout(){};
void Shader::Setup(){}
void Shader::Cleanup(){}
