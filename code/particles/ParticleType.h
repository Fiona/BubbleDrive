/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Oct 2014 by Fiona ****
 ***************************************/

#pragma once
#ifndef _PARTICLETYPE_H_
#define _PARTICLETYPE_H_

/**
 * Particle types are special Entities that essentially  act
 * as per texture/z/render layer containers for emitters
 * they manually hold the batches responsible for drawing.
 */

// Includes
#include "../core/Entity.h"

/**
 *
 */
class ParticleType : public Entity
{


protected:
    void Update_Batches_And_Object_Indicies(bool remove_current);

public:
    ParticleType();

};

#endif
