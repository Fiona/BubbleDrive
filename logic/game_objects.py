"""
Bubble Drive
Action space RPG
(c) Stompy Blondie 2011/2012 http://stompyblondie.com
"""

# python imports
import random, math

# Game engine imports
from core import *

# Game imports
from consts import *


class Test(World_object):
    def __init__(self, game):
        World_object.__init__(self)
        self.game = game
        
        self.x = 300.0
        self.y = 300.0
        self.z = Z_PARTICLES
        self.image = self.game.core.media.gfx['ships_ship2']
        self.colour = (1.0, 0.0, 0.0)

    def Execute(self):
        pass
    


#######################################
##### BACKGROUND OR OTHER VISUALS #####
#######################################


class Background(Process):
    space_dust = None
    objs = {}
    
    def __init__(self, game):
        Process.__init__(self)
        self.game = game
        self.z = Z_BACKGROUND

        # create the bg objects
        #self.objs = {}
        #for obj in self.game.galaxy.current_solar_system.background_objects:
        #    self.add_new_background_object(obj)
            
        # dusttt
        #self.recreate_space_dust()

        # drawing
        self.draw_strategy = "background"
        self.draw_strategy_nebula_image = str(random.randrange(1, BACKGROUND_NUM_NEBULA_TYPES + 1))
