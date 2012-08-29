/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _BATCHOPERATION_H_
#define _BATCHOPERATION_H_


// Includes
#include <vector>
#include <GL/glew.h>
#include <GL/glu.h>
#include "../core/Entity.h"


/**
 * Created and stored in Batch objects to keep track
 * of the actions that need to do in a particular frame.
 * BatchOperations do not directly do the operations that they
 * represent, this is performed by the Batch object. This is
 * purely for data storage.
 * The Update method is used to change the type of operation
 * that this object represents.
 */
class BatchOperation
{

public:
    Entity* oEntity;
    int iObject_Index;
    bool bRemove_Operation;

    BatchOperation(int object_index, Entity* entity);
    BatchOperation(int object_index);
    void Update(bool remove_operation, Entity* entity);

};

#endif