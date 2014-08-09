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
#include <boost/filesystem.hpp>

#if _WIN32
  #include <windows.h>
#endif

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
    GLint compile_status;

	oGame = Game::Instance();

    bool success_vertex = load_from_file(
        GL_VERTEX_SHADER,
        PATH_RESOURCES + SEPARATOR + PATH_SHADERS + SEPARATOR + shader_file_name + ".vert",
        oVertex_Shader_Program
        );
    if(!success_vertex)
        return;

    bool success_fragment = load_from_file(
        GL_FRAGMENT_SHADER,
        PATH_RESOURCES + SEPARATOR + PATH_SHADERS + SEPARATOR + shader_file_name + ".frag",
        oFragment_Shader_Program
        );
    if(!success_fragment)
        return;

    // Load geometry shader if it exists
    bool have_geometry_shader = false;
    if(boost::filesystem::exists(PATH_RESOURCES + SEPARATOR + PATH_SHADERS + SEPARATOR + shader_file_name + ".geom"))
    {

        bool success_geometry = load_from_file(
            GL_GEOMETRY_SHADER,
            PATH_RESOURCES + SEPARATOR + PATH_SHADERS + SEPARATOR + shader_file_name + ".geom",
            oGeometry_Shader_Program
            );
        if(!success_geometry)
            return;

        have_geometry_shader = true;

    }

    // Create and link shader program, and get shader uniform locations
    oShader_Program = glCreateProgram();
    glAttachShader(oShader_Program, oVertex_Shader_Program);
    if(have_geometry_shader)
        glAttachShader(oShader_Program, oGeometry_Shader_Program);
    glAttachShader(oShader_Program, oFragment_Shader_Program);
    glLinkProgram(oShader_Program);

    int info_log_length = 0;
    int max_length;
    glGetProgramiv(oShader_Program, GL_INFO_LOG_LENGTH, &max_length); 
    std::string info_log(max_length, ' ');
    glGetProgramInfoLog(oShader_Program, max_length, &info_log_length, &info_log[0]);

    if(info_log_length > 0)
    {

	printf("Info log for %s: %s\n", shader_file_name.c_str(), info_log.c_str());

#if _WIN32
	MessageBox(NULL, ("Error linking program - " + shader_file_name + "\n").c_str(), "error", MB_ICONSTOP|MB_SETFOREGROUND);
#endif

    }

    Get_Uniform_Locations();

}


/**
 * Loads a shader from a text file.
 * @param
 */
bool Shader::load_from_file(GLenum shader_type, std::string shader_source_file, GLuint &shader_program)
{

	std::string shader_text = oGame->Load_From_File(shader_source_file);
	const char* shader_code = shader_text.c_str();
	if(shader_code == 0)
	{
		fprintf(stderr, "Error loading shader file.\n");
#if _WIN32
		MessageBox(NULL, "error loading shader file", "error", MB_ICONSTOP|MB_SETFOREGROUND);
#endif
		return false;
	}

    GLint compile_status;
    shader_program = glCreateShader(shader_type);
    glShaderSource(shader_program, 1, (const GLchar**)&shader_code, 0);
    glCompileShader(shader_program);
    glGetShaderiv(shader_program, GL_COMPILE_STATUS, &compile_status);

    if(!compile_status)
    {

        fprintf(stderr, ("Error compiling shader - " + shader_source_file + "\n").c_str());
#if _WIN32
	MessageBox(NULL, ("Error compiling shader - " + shader_source_file + "\n").c_str(), "error", MB_ICONSTOP|MB_SETFOREGROUND);
#endif

        int info_log_length = 0;
        int max_length;
		glGetShaderiv(shader_program, GL_INFO_LOG_LENGTH, &max_length); 
		std::string info_log(max_length, ' ');
		glGetShaderInfoLog(shader_program, max_length, &info_log_length, &info_log[0]);
        if(info_log_length > 0)
			printf("%s\n", info_log.c_str());

        return false;

    }
    
    return true;

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
