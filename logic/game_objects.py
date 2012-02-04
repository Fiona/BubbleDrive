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



class Ship(Physical_object):
    """
    Ships are the main entities in game that shoot at each other and stuff.
    Do self.init() at the start of the process creation and
    self.update() on every frame to ensure position updates.
    """
    ship_type = 0
    thruster_accelerations = {}
    turn_towards = (0, 0)
    primary_weapons = []

    target = None
    
    engine_glows = []
    glow_map = None
    shield_effect = None

    distance_to_turn_towards = 0

    intersystem_jumping_to = None
    intrasystem_jumping_to = None
    bubble_jump_phase = None
    bubble_jump_effect = None
    bubble_jump_particles = None
    bubble_jump_particles_2 = None

    lock_controls = False

    wait_anim = 0    
    
    def __init__(self, game, ship_type = 0):
        Physical_object.__init__(self)
        
        self.game = game
        self.ship_type = ship_type
        self.image = self.game.core.media.gfx['ships_ship' + str(self.ship_type)]
        self.z = Z_SHIPS

        self.thruster_accelerations = {'forward' : 0.0, 'backward' : 0.0, 'left' : 0.0, 'right' : 0.0}
        self.target = None

        self.object_name = SHIP_STATS[self.ship_type]['name']

        self.custom_scale = 1.0
        
        self.width = SHIP_STATS[self.ship_type]['width']
        self.height = SHIP_STATS[self.ship_type]['height']
        self.max_velocity = SHIP_STATS[self.ship_type]['max_speed']

        self.mass = SHIP_STATS[self.ship_type]['mass']
        
        self.max_health = SHIP_STATS[self.ship_type]['health']
        self.health = SHIP_STATS[self.ship_type]['health']
        self.max_shields = SHIP_STATS[self.ship_type]['shields']
        self.shields = SHIP_STATS[self.ship_type]['shields']

        self.engine_glows = []

        #if 'engine_glow' in SHIP_STATS[self.ship_type]:
        #    for x in SHIP_STATS[self.ship_type]['engine_glow']:
        #        self.engine_glows.append(Ship_engine_glow(self.game, self, x))
        
        #if not SHIP_STATS[self.ship_type]['glow_map_colour'] is None:
        #    self.glow_map = Ship_glow_map(self.game, self)

        #if not self.shields is None:
        #    self.shield_effect = Ship_shield_effect(self.game, self)
            
        self.primary_weapons = []
        
        for x in SHIP_STATS[self.ship_type]['weapon_mounts']:
            if x['class'] in [WEAPON_CLASS_LASER]:
                x = dict(x)
                x['type'] = x['default']
                x['reload_count'] = WEAPON_STATS[x['type']]['reload_time']
                self.primary_weapons.append(x)

        # Override object settings
        is_ship = True
        collidable = True
        collision_type = COLLISION_TYPE_RECTANGLE
        show_on_minimap = True
        

    def Execute(self):
        """
        Called every frame. Does everything all ships should do.
        """
        # Entering a bubble
        """
        if self.bubble_jump_phase == BUBBLE_JUMP_PHASE_ENTERING_BUBBLE:
            if self.game.player_ship == self:
                self.game.gui.block_gui_keyboard_input = True
            
            if self.bubble_jump_particles is None:
                self.bubble_jump_particles = self.game.particle_emitters['bubble_jump'].add_point((self.x, self.y))
            if self.bubble_jump_effect is None:
                self.bubble_jump_effect = Bubble_jump_effect(self.game, self)
                
            self.lock_controls = True
            for x in self.thruster_accelerations:
                self.thruster_accelerations[x] = 0
            self.velocity = Vector2D(dir = 0, mag = 0)
            self.turn_towards = self.intrasystem_jumping_to
            
            if self.colour[0] > 0:
                self.colour = (self.colour[0] - .005, self.colour[1] - .005, self.colour[2])
            else:
                if not self.bubble_jump_particles is None:
                    self.game.particle_emitters['bubble_jump'].remove_point(self.bubble_jump_particles)                    
                    self.bubble_jump_particles = None
                    
                if self.custom_scale > 0:
                    self.custom_scale -= .05
                    if self.bubble_jump_effect.custom_scale > 0:
                        self.bubble_jump_effect.custom_scale -= (.05 * SHIP_STATS[self.ship_type]['bubble_jump_size'])
                    else:
                        self.bubble_jump_effect.image = None
                    if not self.glow_map is None:
                        self.glow_map.custom_scale = self.custom_scale
                else:
                    if self.bubble_jump_particles_2 is None:
                        self.bubble_jump_particles_2 = self.game.particle_emitters['bubble_jump_after'].add_point((self.x, self.y), death_timer = 6)
                    self.wait_anim += 1
                    if self.wait_anim == 150:
                        self.bubble_jump_effect.signal(S_KILL)
                        self.bubble_jump_effect = None
                        self.bubble_jump_particles_2 = None
                        self.wait_anim = 0
                        self.bubble_jump_phase = BUBBLE_JUMP_PHASE_TRAVELLING_INTRASYSTEM
                        if self.game.player_ship == self:
                            self.game.gui.fade_toggle(lambda: self.game.gui.open_in_game_menu())
                            self.signal(S_SLEEP)

        # Travelling through a bubble
        elif self.bubble_jump_phase == BUBBLE_JUMP_PHASE_TRAVELLING_INTRASYSTEM:
            self.move_forward(
                BUBBLE_JUMP_INTRASYSTEM_SPEED,
                MyrmidonGame.angle_between_points((self.x, self.y), self.intrasystem_jumping_to)
                )
            if self.get_distance(self.intrasystem_jumping_to) < 100:
                self.drop_out_of_bubble()

        # Leaving a bubble
        elif self.bubble_jump_phase == BUBBLE_JUMP_PHASE_LEAVING_BUBBLE:
            if self.bubble_jump_particles_2 is None:
                self.bubble_jump_particles_2 = self.game.particle_emitters['bubble_jump_after'].add_point((self.x, self.y), death_timer = 6)
                
            if self.custom_scale < 1.0:
                self.colour = (self.colour[0] + .03, self.colour[1] + .03, self.colour[2])
                self.custom_scale += .05
                if not self.glow_map is None:
                    self.glow_map.custom_scale = self.custom_scale
            else:
                self.colour = (1.0, 1.0, 1.0)
                self.custom_scale = 1.0
                if not self.glow_map is None:
                    self.glow_map.custom_scale = self.custom_scale
                self.bubble_jump_particles_2 = None
                self.bubble_jump_phase = None
                self.intrasystem_jumping_to = None
                self.lock_controls = False
                if self.game.player_ship == self:
                    MyrmidonGame.engine['input'].mouse.alpha = 1.0                    
                    self.game.gui.block_gui_keyboard_input = False
        """
        
        # Cap accell
        for x in self.thruster_accelerations:
            if self.thruster_accelerations[x] > SHIP_STATS[self.ship_type]['max_acceleration']:
                self.thruster_accelerations[x] = SHIP_STATS[self.ship_type]['max_acceleration']
            if self.thruster_accelerations[x] < -SHIP_STATS[self.ship_type]['max_acceleration']:
                self.thruster_accelerations[x] = -SHIP_STATS[self.ship_type]['max_acceleration']
            self.thruster_accelerations[x] *= 0.95

        # Increase velocity via thrusters
        self.velocity += Vector2D(math.radians(self.rotation), self.thruster_accelerations['forward'], True)
        self.velocity -= Vector2D(math.radians(self.rotation), self.thruster_accelerations['backward'], True)
        self.velocity += Vector2D(math.radians(self.rotation + 90), self.thruster_accelerations['right'], True)
        self.velocity += Vector2D(math.radians(self.rotation - 90), self.thruster_accelerations['left'], True)

        # Turn towards the location we want to
        self.distance_to_turn_towards = self.game.core.get_distance(self.x, self.y, *self.turn_towards)
        if self.distance_to_turn_towards > 32:
            rotation_towards_target = self.game.core.angle_between_points(
                self.x, self.y,
                self.turn_towards[0], self.turn_towards[1]
                )
            self.rotation = self.game.core.near_angle(
                self.rotation,
                rotation_towards_target,
                SHIP_STATS[self.ship_type]['turn_rate']
                )

        # Update weapon related crap
        for x in self.primary_weapons:
            x['reload_count'] += 1
            

    def attempt_to_fire_primary(self):
        """
        Will fire primaries if they're available.
        """
        for x in self.primary_weapons:
            if x['reload_count'] >= WEAPON_STATS[x['type']]['reload_time']:
                #Primary_weapon(self.game, self, x['point'], x['type'])
                x['reload_count'] = 0
    

    def thrust(self, reverse = False):
        """
        Call if the ship is attempting to accellerate forward.
        """
        if reverse:
            self.thruster_accelerations['backward'] += SHIP_STATS[self.ship_type]['acceleration']
        else:
            self.thruster_accelerations['forward'] += SHIP_STATS[self.ship_type]['acceleration']


    def thrust_sideways(self, right = False):
        """
        Call if the ship is attempting to accellerate sideways.
        """
        self.thruster_accelerations['left' if not right else 'right'] += SHIP_STATS[self.ship_type]['acceleration']
            

    def change_target(self, new_target):
        self.target = new_target



class Player_ship(Ship):
    """
    Main player ship, generally always followed by the camera,
    gets shot at, shoots things - hopefully doesn't explode!
    """    
    def __init__(self, game):        
        Ship.__init__(self, game, ship_type = SHIP_TYPE_PROSPERO_FIGHTER)
        self.faction = FACTION_PLAYER
        self.x = 0.0#50000
        self.y = 0.0#50000
        self.object_name = ""
        self.custom_scale = .9
        self.init()
        

    def Execute(self):
        Ship.Execute(self);
        #print self.x, ",", self.y
        #print self.game.core.camera_x, ",", self.game.core.camera_y

        if not self.lock_controls:

            # Press forward key
            for _key in [self.game.settings['key_ship_forward'], key.UP]:
                if self.game.core.Keyboard_key_down(_key):
                    print "forward"
                    self.thrust()
                    break

            # Press back key
            for _key in [self.game.settings['key_ship_back'], key.DOWN]:
                if self.game.core.Keyboard_key_down(_key):
                    print "down"
                    self.thrust(reverse = True)
                    break

            # Press left
            for _key in [self.game.settings['key_ship_left'], key.LEFT]:
                if self.game.core.Keyboard_key_down(_key):
                    print "left"
                    self.thrust_sideways()
                    break

            # Press right
            for _key in [self.game.settings['key_ship_right'], key.RIGHT]:
                if self.game.core.Keyboard_key_down(_key):
                    print "right"
                    self.thrust_sideways(right = True)
                    break

            # Press fire key
            if self.game.core.mouse.left_down:
                self.attempt_to_fire_primary()

            # Turn towards mouse
            self.turn_towards = self.game.core.screen_to_world(self.game.gui.mouse.x, self.game.gui.mouse.y)
            #print self.turn_towards



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
        self.recreate_space_dust()

        # drawing
        self.draw_strategy = "background"
        self.draw_strategy_nebula_image = str(random.randrange(1, BACKGROUND_NUM_NEBULA_TYPES + 1))


    def recreate_space_dust(self, start_sleeped = False):
        if not self.space_dust is None:
            self.space_dust.Kill()
            
        self.space_dust = Space_dust(self.game)

        if start_sleeped:
            self.space_dust.Stop_logic()
            self.space_dust.Stop_drawing()


    def On_exit(self):
        self.space_dust.Kill()



class Space_dust(World_object):
    
    dust_items = {}
    
    def __init__(self, game):
        World_object.__init__(self)
        self.game = game
        self.z = Z_SPACE_DUST
        self.init()

        self.screen_size_adjust = ((self.game.settings['screen_width']/2) / MIN_ZOOM_LEVEL), ((self.game.settings['screen_height']/2) / MIN_ZOOM_LEVEL)
        self.max_screen_size = ((self.game.settings['screen_width'] / MIN_ZOOM_LEVEL), (self.game.settings['screen_height'] / MIN_ZOOM_LEVEL))

        for image_num in range(1, SPACE_DUST_TYPES + 1):
            self.dust_items[image_num] = {}
            for x in range(SPACE_DUST_NUM):
                pos = (random.randrange(int(self.game.camera.x - self.screen_size_adjust[0]), int(self.game.camera.x + self.screen_size_adjust[0])),
                       random.randrange(int(self.game.camera.y - self.screen_size_adjust[1]), int(self.game.camera.y + self.screen_size_adjust[1])))
                self.dust_items[image_num][x] = pos

        self.draw_strategy = "space_dust"
