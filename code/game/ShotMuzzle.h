/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************/

#pragma once
#ifndef _SHOTMUZZLE_H_
#define _SHOTMUZZLE_H_

/**
 * Shot muzzle object header
 */


// Includes
#include <memory>
#include "../core/Entity.h"


/**
 *
 */
class ShotMuzzle : Entity
{

private:

	int Life;
    Light Muzzle_Light;
    Ship* Parent;
    int Mount_Point_Num;

    void Update_Pos();
    
public:

    ShotMuzzle(Ship* parent_ship, int mount_point_num, int rot);
    void Logic();

};

#endif
