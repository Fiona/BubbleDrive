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
#include "../core/Entity.h"
#include "../utils/Vector2D.h"


/**
 *
 */
class Ship : public Entity
{

private:

    Vector2D* Velocity;
	float Max_Velocity;
	float Velocity_Friction;
	float Acceleration;
	float Max_Acceleration;
	float Turn_Rate;
	std::map<char, float>* Thrusters;
	int Reload_Time;
    
public:

    Ship();
    void Logic();

    std::vector<float> Get_Mount_Point_Location(int mount_point_num);

};

#endif
