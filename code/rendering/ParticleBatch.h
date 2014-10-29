/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Oct 2014 by Fiona ****
 ***************************************/

#pragma once
#ifndef _PARTICLEBATCH_H_
#define _PARTICLEBATCH_H_

/**
 * Special batch type for particles
 */


// Includes
#include "../rendering/Batch.h"

// Forward dcl


/**
 *
 */
class ParticleBatch : public Batch
{

public:
    ParticleBatch();
    void init(float z, int render_layer, GLuint texture);

};

#endif
