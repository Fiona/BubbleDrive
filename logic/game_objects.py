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
from helpers import *


class Camera(Process):
    # Use start_cam_shake() to shake the camera. This is the number of frames
    # we jiggle about for.
    shake_cam = 0

    # Anchor can be set to a MyrmidonProcess object - it's usually the player ship
    # Change anchor with set_anchor_to() with an optional offset.
    anchor = None
    anchor_offset = [0,0]

    # Keeps track of if we're currently shifting somewhere. Only works with anchors unless we set an offset.
    shifting = False
    shifting_previous_anchor_offset = [0,0]
    shifting_start_point = None

    # We can specify a callback to call once we've finished moving.
    shifting_callback = False

    # Without an anchor we use this to freely move the cam with the mouse and keep track of speed
    speed = [0.0, 0.0]

    # Can be accessed to find out what the location was this frame before it changed
    previous_location = [0.0, 0.0]

    def __init__(self, game):
        Process.__init__(self)
        self.game = game
        self.priority = PRIORITY_CAMERA
        self.iter = 0
        
        
    def Execute(self):
        self.previous_location = [self.x, self.y]

        # Shuffle the camera if we are moving
        if self.anchor and self.shifting:
            if self.iter < MOVE_CAMERA_TIME:
                self.iter += 1
                self.previous_location = [self.x, self.y]
                # For each tick of the shift we interpolate from the starting point to the end point if required
                if not self.shifting_start_point == None:
                    self.x = lerp(i, MOVE_CAMERA_TIME, self.shifting_start_point[0], self.anchor.x)
                    self.y = lerp(i, MOVE_CAMERA_TIME, self.shifting_start_point[1], self.anchor.y)
                else:
                    self.x = self.anchor.x
                    self.y = self.anchor.y
                # We add on the offset as we go along if we need it
                if not self.shifting_previous_anchor_offset == self.anchor_offset:
                    self.x += lerp(i, MOVE_CAMERA_TIME, self.shifting_previous_anchor_offset[0], self.anchor_offset[0])
                    self.y += lerp(i, MOVE_CAMERA_TIME, self.shifting_previous_anchor_offset[1], self.anchor_offset[1])
                self.game.core.camera_x = self.x
                self.game.core.camera_y = self.y
                return
            else:
                self.shifting = False
                self.shifting_start_point = None
                self.shifting_previous_anchor_offset = [0, 0]
                if not self.shifting_callback == None:
                    self.shifting_callback()
                self.iter = 0
        # if not shuffling we just centre on the offset
        else:                
            if not self.anchor == None:
                self.x = self.anchor.x + self.anchor_offset[0]
                self.y = self.anchor.y + self.anchor_offset[1]
                self.speed = [0.0, 0.0]

        # Shakey cam
        if self.shake_cam > 0:
            self.shake_cam -= 1
            self.x += random.randrange(-3,3)
            self.y += random.randrange(-3,3)

        self.game.core.camera_x = self.x
        self.game.core.camera_y = self.y


    def set_anchor_to(self, anchor_object = None, offset = [0, 0], instant = False, callback = None):
        """
        Call this to set the camera anchor. We can anchor to a process. Pass in the instance
        of the object we want to centre the camera on as the first parameter.
        offset is a tuple containing x,y - the offset we want for the camera.
        instant, if set to True, will immediately make the camera bolt to the anchor rather than gradually shift.
        callback can be a function we want to call after the camera has arrived at it's destination.
        """
        # If we have no anchor now then there's no reason to shuffle to it
        if not anchor_object == None and not self.anchor == anchor_object and instant == False:
            self.shifting_start_point = [self.x, self.y]
            self.shifting = True
        else:
            self.shifting_start_point = None

        if not offset == self.anchor_offset and instant == False:
            self.shifting_previous_anchor_offset = self.anchor_offset
            self.shifting = True
            
        self.anchor = anchor_object
        self.anchor_offset = offset
        self.shifting_callback = callback


    def start_cam_shake(self, time = SHAKE_CAM_TIME):
        """
        Call if we want the camera to start shaking. We can optionally
        pass in how many frames we want to shake for, otherwise it will be
        whatever the default time is.
        """
        self.shake_cam = time



class Test(World_object):
    
    def __init__(self, game):
        World_object.__init__(self)
        self.init()
        self.game = game
        self.x = 300.0
        self.y = 300.0
        self.z = Z_PARTICLES
        self.image = self.game.core.media.gfx['ships_ship2']
        self.colour = (1.0, 0.0, 0.0)
        self.health = 500

    def Execute(self):
        for x in self.game.core.world_objects:
            print x.health


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
