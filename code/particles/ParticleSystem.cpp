/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Oct 2014 by Fiona ****
 ***************************************/

/**
   Particle system source
 */

// Includes
#include "ParticleSystem.h"
#include "ParticleType.h"
#include "../core/Game.h"


/**
 * Constructor
 */
ParticleSystem::ParticleSystem()
{

    oGame.reset(Game::Instance());

    // Create initial particle systems
    Particle_Types["test"] = std::shared_ptr<ParticleType>(new ParticleType());

}


/**
 * Destructor
 */
ParticleSystem::~ParticleSystem(){ }
