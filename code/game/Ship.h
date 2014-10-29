/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Feb 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _SHIP_H_
#define _SHIP_H_

/**
 * Ship object header
 */


// Includes
#include <map>
#include <memory>
#include "../core/Entity.h"

// forward decl
class Vector2D;

/**
 *
 */
class Ship : public Entity
{

private:

	float Max_Velocity;
	float Velocity_Friction;
	float Acceleration;
	float Max_Acceleration;
	float Turn_Rate;
	std::map<char, float> Thrusters;
	int Reload_Time;
    
public:

    Ship();
    void Logic();
    std::vector<float> Get_Mount_Point_Location(int mount_point_num);

    std::unique_ptr<Vector2D> Velocity;

};

#endif
