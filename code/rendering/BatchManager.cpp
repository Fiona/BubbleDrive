/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

/**
 * BatchManager object source file.
 */

// Includes
#include <algorithm>
#include "BatchManager.h"


/**
 * Constructor
 */
BatchManager::BatchManager()
{

	oGame = Game::Instance();

	iCurrently_Bound_VBO = -1;

}


/**
 * Deconstructor
 */
BatchManager::~BatchManager()
{

    for(std::vector<Batch*>::iterator it = oBatches.begin(); it != oBatches.end(); ++it)
    {
        if(*it == NULL)
            continue;
        delete(*it);
    }

}


/**
 * Called by Entity objects to ask for new object positions in 
 * a relevant Batch.
 * Returns a strided vector containing Batch numbers (that correspond
 * to their location within the oBatches vector) and an object index,
 * one for each requested object.
 */
void BatchManager::Request_New_Batch_And_Object_Indicies(Entity* entity, int num_objects_requested, std::vector<int>* batch_and_object_indicies)
{

	// Iterate through each required object and each batch to
	// try and find a batch this will work for us.
	int batch_num, found_batch, object_index;

	for(int obj_num = 1; obj_num <= num_objects_requested; obj_num++)
	{

		if(!entity->Get_Texture_Num_For_Object_Num(obj_num))
			continue;

		found_batch = -1;
		object_index = -1;

		for(batch_num = 0; batch_num < (int)oBatches.size(); batch_num++)
		{	

			// If this batch is good for us then request an object index from it
			if(
				oBatches[batch_num]->iZ == entity->Get_Z() &&
				oBatches[batch_num]->oTexture == entity->Get_Texture_Num_For_Object_Num(obj_num) &&
				oBatches[batch_num]->iRender_Layer == entity->Get_Render_Layer()
				)
			{

				object_index = oBatches[batch_num]->Request_Object_Index();

				// If we found one (-1 indicates the batch is full) then we can use 
				// this batch and break.
				if(object_index > -1)
				{
					found_batch = batch_num;
					break;
				}

			}

		}

		// If a suitable batch was not found then we clearly need a new one.
		if(found_batch == -1)
		{
			found_batch = Create_New_Batch(
				entity->Get_Z(),
				entity->Get_Texture_Num_For_Object_Num(obj_num),
				entity->Get_Render_Layer()
				);
			object_index = oBatches[found_batch]->Request_Object_Index();
		}

		// By now we will have a suitable batch and index for this object
		batch_and_object_indicies->push_back(found_batch);
		batch_and_object_indicies->push_back(object_index);

	}

}


/**
 * Entities call this when their VBO values need updating. For 
 * instance if they change their colour, scale, rotation or position.
 */
void BatchManager::Request_Object_Update_For_Entity(Entity* entity, std::vector<int>* batches_and_object_indicies)
{

	int object_num = 1;

	for(int vector_index = 0; vector_index < (int)batches_and_object_indicies->size(); vector_index+=2)
	{

		oBatches[(*batches_and_object_indicies)[vector_index]]->New_Update_Batch_Operation(
			entity,
			(*batches_and_object_indicies)[vector_index+1],
			object_num		
		);

		object_num++;

	}

}


/**
 * Entites call this when they're killed to ensure that they are cleaned
 * up within the VBOs. Also it is called when they change Image, Z, Render Mode or
 * the number of objects they require changes. This is done so they can request
 * new object indexes.
 */
void BatchManager::Request_Removal_Of_Objects(std::vector<int>* batches_and_object_indicies, bool update_dupes)
{

	for(int vector_index = 0; vector_index < (int)batches_and_object_indicies->size(); vector_index+=2)
	{
		oBatches[(*batches_and_object_indicies)[vector_index]]->Free_Object_Index((*batches_and_object_indicies)[vector_index+1]);
		oBatches[(*batches_and_object_indicies)[vector_index]]->New_Remove_Batch_Operation((*batches_and_object_indicies)[vector_index+1], update_dupes);
	}

}


/**
 * Called by the core Game object every frame. Binds the Batches in
 * Z order, tells them to update and then finally to draw.
 */
void BatchManager::Update_And_Render_Batches()
{

	iCurrently_Bound_VBO = -1;
	oGame->iCurrent_Bound_Texture = -1;

    for(std::vector<Batch*>::iterator batch = oBatches_In_Layer_And_Z_Order.begin(); batch != oBatches_In_Layer_And_Z_Order.end(); ++batch)
    {

		(*batch)->Bind(iCurrently_Bound_VBO);
		(*batch)->Run_Batch_Operations();
		(*batch)->Draw();

	}

}


/**
 * Used to create new Batch objects when a suitable one is
 * not found.
 * Returns the new batch index in the vector.
 */
int BatchManager::Create_New_Batch(float z, GLuint texture, int render_layer)
{

	Batch* new_batch = new Batch(z, render_layer, texture);

	oBatches.push_back(new_batch);
	oBatches_In_Layer_And_Z_Order.push_back(new_batch);

	// reorder batches so they're in layer order then by z 
	std::sort(oBatches_In_Layer_And_Z_Order.begin(), oBatches_In_Layer_And_Z_Order.end(), &BatchManager::Sort_Batches_By_Layer_And_Z);

	return (int)(oBatches.size() - 1);

}


/**
 * Sort method used by std::sort to sort Batch objects by
 * their Z value.
 */
bool BatchManager::Sort_Batches_By_Layer_And_Z(Batch* i, Batch* j)
{
	if(i->iRender_Layer == j->iRender_Layer)
		return (i->iZ > j->iZ);
	return (i->iRender_Layer > j->iRender_Layer);
}