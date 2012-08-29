/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Aug 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _BATCHMANAGER_H_
#define _BATCHMANAGER_H_


// Includes
#include <vector>
#include <GL/glew.h>
#include <GL/glu.h>
#include "../core/Game.h"
#include "Batch.h"
#include "../core/Entity.h"
#include "RenderMode.h"


// Forward declaration
class RenderMode;
class Batch;


/**
 * The core batch manager. A single instance of this will
 * be created in the Game instance.
 * Entities request batch indicies from it using a variety of
 * methods.
 * The main Game instance will call Update_And_Render_Batches once
 * per-frame, which will iterate through all batches, make sure the
 * VBOs are all in line and finally render them all using the
 * relevant RenderMode.
 *
 * Note - The term "Object" is a nebulous concept that basically
 *   refers to a single group of 6 verticies that are used to draw
 *   two triangles to the screen. It is used in various places through
 *   out the rendering system.
 * Note - If an Entity changes it's image, render mode, Z  or num
 *   of objects to render then it is required that it first requests
 *   to remove them before requesting new indicies.
 */
class BatchManager
{

public:
    GLuint iCurrently_Bound_VBO;
	int iCurrent_Render_Mode;

    BatchManager();
    ~BatchManager();
	void Request_New_Batch_And_Object_Indicies(Entity* entity, int num_objects_requested, std::vector<int>* batch_and_object_indicies);
	void Request_Object_Update_For_Entity(Entity* entity, std::vector<int>* batches_and_object_indicies);
	void Request_Removal_Of_Objects(std::vector<int>* batches_and_object_indicies, bool update_dupes);
    void Update_And_Render_Batches();
	void Set_Current_Render_Mode(int render_mode);

private:
	Game* oGame;
	std::vector<Batch*> oBatches;
	std::vector<Batch*> oBatches_In_Z_Order;
	std::map<int, RenderMode*> oRender_Modes;

	int Create_New_Batch(float z, GLuint texture, int render_mode);    
	static bool Sort_Batches_By_Z(Batch* i, Batch* j);

};

#endif