/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

/**
 * Batch object source file.
 */

// Includes
#include "../core/consts.h"
#include "Batch.h"


/**
 * Constructor. Will create the VBO to default values.
 */
Batch::Batch(float z, int render_layer, GLuint texture)
{

	iZ = z;
    iRender_Layer = render_layer;
    oTexture = texture;	

	oGame = Game::Instance();

	// Create VAO 
	glGenVertexArrays( 1, &oVAO);

	// create VBO and load it with data
	glGenBuffers(1, &oVBO);
	glBindBuffer(GL_ARRAY_BUFFER, oVBO);

	int num_elements = (NUM_OBJECTS_IN_VBO * NUM_VERTEX_IN_OBJECT * NUM_ELEMENTS_PER_VERTEX);
	std::vector<GLfloat> vbo_data(num_elements, 0.0);
	glBufferData(
		GL_ARRAY_BUFFER,
		num_elements * sizeof(GLfloat),
		&vbo_data[0],
		GL_STREAM_DRAW
		);
	vbo_data.clear();

	// Specify layout of vertex data
	glBindVertexArray(oVAO);
	glBindBuffer(GL_ARRAY_BUFFER, oVBO);
	oGame->oRenderer->Specify_Vertex_Layout_For_Render_Layer(iRender_Layer);

	// Create pool
	for(int i = NUM_OBJECTS_IN_VBO-1; i >= 0; i--)
		oAvailable_Object_Indicies.push_back(i);

}


/**
 * Deconstructor
 */
Batch::~Batch()
{
	glDeleteBuffers(1, &oVBO);
	glDeleteVertexArrays(1, &oVAO);
}


/**
 * Asks the Batch for a new object index.
 * Mostly just pops a free index from the pool and returns
 * it. -1 is returned if there is nothing left in the pool.
 */
int Batch::Request_Object_Index()
{

	if(oAvailable_Object_Indicies.size() == 0)
		return -1;

	int new_index = oAvailable_Object_Indicies.back();
	oAvailable_Object_Indicies.pop_back();
	oAssigned_Object_Indicies.insert(new_index);
	iLargest_Object_Index = *std::max_element(oAssigned_Object_Indicies.begin(), oAssigned_Object_Indicies.end());

	return new_index;

}


/**
 * Used by the BatchManager to tell the Batch that an update will
 * be required for the supplied Entity at the supplied object index.
 * Checks for duplicates and updates removal operations.
 */
void Batch::New_Update_Batch_Operation(Entity* entity, int object_index, int entity_object_number)
{

	// Look in current operations for this object index
	for(std::vector<BatchOperation*>::iterator op = oBatch_Operations.begin(); op != oBatch_Operations.end(); ++op)
    {

		if((*op)->iObject_Index == object_index)
		{

			// If this is a remove op then we need to change it to an update one
			if((*op)->bRemove_Operation)
				(*op)->Update(false, entity, entity_object_number);

			// Dupe found
			return;

		}

	}

	// No dupe was found so we need a new one
	oBatch_Operations.push_back(
		new BatchOperation(object_index, entity, entity_object_number)
		);

}


/**
 * Used by the BatchManager to tell the Batch that the object index
 * supplied is going to be zeroed out. Checks for duplicate removal
 * operations.
 */
void Batch::New_Remove_Batch_Operation(int object_index, bool update_dupes)
{

	// Look in current operations for this object index and quit
	// if a dupe was found
	if(update_dupes)
	{
		for(std::vector<BatchOperation*>::iterator op = oBatch_Operations.begin(); op != oBatch_Operations.end(); ++op)
		{
			if((*op)->iObject_Index == object_index && !(*op)->bRemove_Operation)
			{
				(*op)->Update(true, NULL, 1);
				return;
			}
		}
	}

	// No dupe was found so we need a new one
	oBatch_Operations.push_back(
		new BatchOperation(object_index)
		);

}


/**
 * Used by the BatchManager to tell the Batch to immediately return the
 * supplied object index to the pool ready to be used again.
 */
void Batch::Free_Object_Index(int object_index)
{

	oAvailable_Object_Indicies.push_back(object_index);
	oAssigned_Object_Indicies.erase(object_index);

	if(oAssigned_Object_Indicies.size() > 0)
		iLargest_Object_Index = *std::max_element(oAssigned_Object_Indicies.begin(), oAssigned_Object_Indicies.end());
	else
		iLargest_Object_Index = 0;

	// Sort indicies in reverse order
	//std::sort(oAvailable_Object_Indicies.begin(), oAvailable_Object_Indicies.end(), std::greater<int>());

}


/**
 * Called before updating and drawing. Simply binds the VBO.
 */
void Batch::Bind(int currently_bound)
{

	glBindVertexArray(oVAO);

	oGame->oRenderer->Set_Current_Render_Layer(iRender_Layer);

    if(oGame->iCurrent_Bound_Texture != oTexture)
    {
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, oTexture);
        oGame->iCurrent_Bound_Texture = oTexture;
    }

}


/**
 * Iterates through the saved operations and updates the 
 * VBO values where necessary. Requesting values from Entities if
 * updating.
 */
void Batch::Run_Batch_Operations()
{

	if(oBatch_Operations.size() == 0)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, oVBO);

	GLfloat vbo_data[NUM_VERTEX_IN_OBJECT * NUM_ELEMENTS_PER_VERTEX];

	for(std::vector<BatchOperation*>::iterator op = oBatch_Operations.begin(); op != oBatch_Operations.end(); ++op)
	{
	
		if((*op)->bRemove_Operation)
		{
			for(int i = 0; i < NUM_VERTEX_IN_OBJECT * NUM_ELEMENTS_PER_VERTEX; i++)
				vbo_data[i] = 0;
		}
		else
		{
			(*op)->oEntity->Get_Object_Index_Data((*op)->iObject_Index, vbo_data, (*op)->iEntity_Object_Num);
		}
		glBufferSubData(
			GL_ARRAY_BUFFER,
			(*op)->iObject_Index * NUM_VERTEX_IN_OBJECT * NUM_ELEMENTS_PER_VERTEX * sizeof(GLfloat),
			NUM_VERTEX_IN_OBJECT * NUM_ELEMENTS_PER_VERTEX * sizeof(GLfloat),
			&vbo_data
		);

	}

	while(!oBatch_Operations.empty())
	{
		delete(oBatch_Operations.back());
		oBatch_Operations.pop_back();
	}

}


/**
 * Finally draws the Batch.
 */
void Batch::Draw()
{

	glDrawArrays(GL_TRIANGLES, 0, (iLargest_Object_Index + 1) * NUM_VERTEX_IN_OBJECT);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

