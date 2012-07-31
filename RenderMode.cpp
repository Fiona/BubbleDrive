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

	oUniforms.insert(
		std::pair<std::string, GLint>(
			"texture_num",
			glGetUniformLocation(oShader_Program, "texture_num")
			)
		);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"position",
			glGetUniformLocation(oShader_Program, "position")
			)
		);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"rotation",
			glGetUniformLocation(oShader_Program, "rotation")
			)
		);
	oUniforms.insert(
		std::pair<std::string, GLint>(
			"scale",
			glGetUniformLocation(oShader_Program, "scale")
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

	glUseProgram(oShader_Program);
	glUniform1i(oUniforms["texture_num"], 0);
	float screen_size[2]; screen_size[0] = DEFAULT_SCREEN_WIDTH; screen_size[1] = DEFAULT_SCREEN_HEIGHT;
	glUniform2fv(oUniforms["screen_size"], 1, screen_size);
	glUseProgram(0);

}


/**
 * Used by the render set up to set uniform vals.
 */
void RenderMode::Set_Uniform_Values(Entity* entity)
{

	glUniform4fv(oUniforms["position"], 1, entity->aPosition);
	glUniform1f(oUniforms["rotation"], entity->Get_Rotation());
	glUniform1f(oUniforms["scale"], entity->Get_Scale());
	glUniform4fv(oUniforms["camera_position"], 1, oGame->aCamera_Position);

}


/**
 * Does everything that is required before rendering.
 * You have to pass the Entity object in directly.
 */
void RenderMode::Setup_Render_Pass(Entity* entity)
{

	// Binding texture
    if(oGame->iCurrent_Bound_Texture != entity->Get_Image()->iTexture_Num)
    {
	    glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, entity->Get_Image()->iTexture_Num);
        oGame->iCurrent_Bound_Texture = entity->Get_Image()->iTexture_Num;
    }

	glUseProgram(oShader_Program);
	Set_Uniform_Values(entity);

	glBindBuffer(GL_ARRAY_BUFFER, entity->aVBO[1]);
	glColorPointer(4, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, entity->aVBO[0]);
	glVertexPointer(2, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, entity->aVBO[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

}


/**
 * Actually does the rendering.
 * You have to pass the Entity object in directly.
 */
void RenderMode::Do_Render_Pass(Entity* entity)
{

	glDrawArrays(GL_TRIANGLES, 0, 6);

}


/**
 * Cleans up, disables GL modes etc
 * You have to pass the Entity object in directly.
 */
void RenderMode::Cleanup_Render_Pass(Entity* entity)
{

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY); 

	glUseProgram(0);

}
