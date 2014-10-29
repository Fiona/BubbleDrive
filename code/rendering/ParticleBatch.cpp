/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started Oct 2014 by Fiona ****
 ***************************************/

/**
   Particle batch source
 */

// Includes
#include "ParticleBatch.h"


/**
 * Constructor
 */
void ParticleBatch::init(float z, int render_layer, GLuint texture)
{

	iZ = z;
    iRender_Layer = render_layer;
    oTexture = texture;	

}
