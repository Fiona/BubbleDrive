/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

/**
 * BatchOperation object source file.
 */

// Includes
#include "BatchOperation.h"


/**
 * Constructor. Use when creating an update operation.
 */
BatchOperation::BatchOperation(int object_index, Entity* entity, int entity_object_number)
{

	oEntity = entity;
    iObject_Index = object_index;
	iEntity_Object_Num = entity_object_number;
    bRemove_Operation = false;

}


/**
 * Constructor. Use when creating an update operation.
 */
BatchOperation::BatchOperation(int object_index)
{

	oEntity = 0;
    iObject_Index = object_index;
	iEntity_Object_Num = 0;
    bRemove_Operation = true;

}


/**
 * Use this when you need to change what type of operation
 * this is.
 */
void BatchOperation::Update(bool remove_operation, Entity* entity, int entity_object_number)
{

	oEntity = entity;
	iEntity_Object_Num = entity_object_number;
    bRemove_Operation = remove_operation;

}
