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
#include "../core/Entity.h"


/**
 *
 */
class Test : public Entity
{

private:
    int iType;
	Light* oMy_Light;
	int life;

public:

    Test(int type);
    void Logic();

};

#endif
