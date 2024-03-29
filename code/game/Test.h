/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Nov 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _TEST_H_
#define _TEST_H_

/**
 * Test object header
 */


// Includes
#include <memory>
#include "../core/Entity.h"

// Forward decl
class Light;

/**
 *
 */
class Test : public Entity
{

private:
    int iType;
	int life;
    std::shared_ptr<Light> oMy_Light;

public:

    Test(int type);
    void Logic();

};

#endif
