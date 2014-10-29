/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Oct 2014 by Fiona ****
 ***************************************/

#pragma once
#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

/**
 * Main particle system object, everything else should interact with this
 */

// Includes
#include <map>
#include <memory>
#include <string>

// Forward decl
class Game;
class ParticleType;

/**
 *
 */
class ParticleSystem
{

public:
    std::shared_ptr<Game> oGame;

    ParticleSystem();
    ~ParticleSystem();

private:
    std::map<std::string, std::shared_ptr<ParticleType>> Particle_Types;

};

#endif
