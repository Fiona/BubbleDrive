/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _BATCH_H_
#define _BATCH_H_


// Includes
#include <vector>
#include <set>
#include <GL/glew.h>
#include <GL/glu.h>
#include "BatchOperation.h"
#include "../core/Entity.h"
#include "../core/Game.h"


/**
 * A Batch object is responsible for a VBO that corresponds
 * to a single image, Z layer and render mode.
 * Entity objects should not interact directly with the Batch objects
 * and instead should make requests to the BatchManager object.
 * The BatchManager will request various things of Batch objects.
 * For instance, requesting future operations on the VBO and requesting
 * that the VBO be finally updated and drawn.
 * Batch objects do interact with Entity objects be requesting their
 * vertex data using the Get_Object_Index_Data method that Entities have.
 */
class Batch
{

private:
	Game* oGame;
    GLuint oVBO;
	GLuint oVAO;
	int iLargest_Object_Index;

	std::vector<BatchOperation*> oBatch_Operations;
	std::vector<int> oAvailable_Object_Indicies;
	std::set<int> oAssigned_Object_Indicies;

public:
	float iZ;
    int iRender_Layer;
    GLuint oTexture;

	Batch(float z, int render_layer, GLuint texture);
    ~Batch();
    int Request_Object_Index();
    void New_Update_Batch_Operation(Entity* entity, int object_index, int entity_object_number);
    void New_Remove_Batch_Operation(int object_index, bool update_dupes);
    void Free_Object_Index(int object_index);
    void Bind(int currently_bound);
    void Run_Batch_Operations();
    void Draw();

};

#endif