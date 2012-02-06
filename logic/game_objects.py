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
        self.is_ship = True
        
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

        if 'engine_glow' in SHIP_STATS[self.ship_type]:
            for x in SHIP_STATS[self.ship_type]['engine_glow']:
                self.engine_glows.append(Ship_engine_glow(self.game, self, x))
        
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
                Primary_weapon(self.game, self, x['point'], x['type'])
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



class Ship_engine_glow(World_object):
    steady_scale_dir = 1
    steady_scale_wait = 0
    
    def __init__(self, game, ship, glow_info):
        World_object.__init__(self)
        self.game = game
        self.ship = ship
        self.z = self.ship.z + 1
        self.blend = True
        self.glow_info = glow_info
        self.colour = self.glow_info['colour']
        self.alpha = .9
        self.scale = 0
        self.init()
        
        self.image = self.game.core.media.gfx[str('fx_engine_glow' + str(glow_info['type']))]
        self.rot_point = glow_info['point']
        self.custom_scale = 1.0

    def Execute(self):
        amt = ((SHIP_STATS[self.ship.ship_type]['max_acceleration'] * self.ship.thruster_accelerations[self.glow_info['dir']]) * 100)
        self.alpha = .4 * amt
        self.custom_scale = self.glow_info['scale'] * amt

        if self.custom_scale > (self.glow_info['scale']-.1):
            if self.steady_scale_dir == 1:
                self.custom_scale = lerp(self.steady_scale_wait, 10, (self.glow_info['scale']-.05), self.glow_info['scale'])
                self.steady_scale_wait += 1
                if self.steady_scale_wait == 10:
                    self.steady_scale_dir = 0
                    self.steady_scale_wait = 0
            else:
                self.custom_scale = lerp(self.steady_scale_wait, 10, self.glow_info['scale'], (self.glow_info['scale'] - .05))
                self.steady_scale_wait += 1
                if self.steady_scale_wait == 10:
                    self.steady_scale_dir = 1
                    self.steady_scale_wait = 0
        else:
            self.steady_scale_dir = 1
            self.steady_scale_wait = 0

        if self.alpha > 0:
            self.rotation = self.ship.rotation + self.glow_info['rotation']
            point = self.game.core.rotate_point(self.rot_point[0] - ((self.image.width/2) * self.custom_scale), self.rot_point[1], self.rotation)
            self.x = self.ship.x + point[0]
            self.y = self.ship.y + point[1]



class Player_ship(Ship):
    """
    Main player ship, generally always followed by the camera,
    gets shot at, shoots things - hopefully doesn't explode!
    """    
    def __init__(self, game):        
        Ship.__init__(self, game, ship_type = SHIP_TYPE_PROSPERO_FIGHTER)
        self.faction = FACTION_PLAYER
        self.x = 50000
        self.y = 50000
        self.object_name = ""
        self.custom_scale = .9
        self.init()
        

    def Execute(self):
        Ship.Execute(self)

        if not self.lock_controls:

            # Press forward key
            for _key in [self.game.settings['key_ship_forward'], key.UP]:
                if self.game.core.Keyboard_key_down(_key):
                    self.thrust()
                    break

            # Press back key
            for _key in [self.game.settings['key_ship_back'], key.DOWN]:
                if self.game.core.Keyboard_key_down(_key):
                    self.thrust(reverse = True)
                    break

            # Press left
            for _key in [self.game.settings['key_ship_left'], key.LEFT]:
                if self.game.core.Keyboard_key_down(_key):
                    self.thrust_sideways()
                    break

            # Press right
            for _key in [self.game.settings['key_ship_right'], key.RIGHT]:
                if self.game.core.Keyboard_key_down(_key):
                    self.thrust_sideways(right = True)
                    break

            # Press fire key
            if self.game.core.mouse.left_down:
                self.attempt_to_fire_primary()

            # Turn towards mouse
            self.turn_towards = self.game.core.screen_to_world(self.game.gui.mouse.x, self.game.gui.mouse.y)



class AI_ship(Ship):    
    strafe_dir = False
    switch_strafe_dir_count = 0
    switch_strafe_dir_wait_til = 0
    
    def __init__(self, game, ship_info):
        Ship.__init__(self, game, ship_info['type'])
        self.game = game
        self.ship_info = ship_info
        self.faction = FACTION_ENEMY
        self.targetable = True        
        self.init()
        self.x = self.ship_info['x']
        self.y = self.ship_info['y']

        self.max_weapon_range = None
        for x in self.primary_weapons:
            if self.max_weapon_range is None or WEAPON_STATS[x['type']]['range'] > self.max_weapon_range:
                self.max_weapon_range = WEAPON_STATS[x['type']]['range']


    def Execute(self):
        Ship.Execute(self)

        if not self.lock_controls:

            if self.target is None:
                self.get_new_target()
            else:
                self.turn_towards = (self.target.x + (self.target.image.width/2), self.target.y + (self.target.image.height/2))

                if self.distance_to_turn_towards > (self.max_weapon_range / 2):
                    self.thrust()
                else:
                    self.thrust_sideways(right = self.strafe_dir)

                if self.distance_to_turn_towards < (self.max_weapon_range / 4):
                    self.thrust(reverse = True)

                if self.distance_to_turn_towards < self.max_weapon_range:
                    self.attempt_to_fire_primary()

                self.switch_strafe_dir_count += 1
                if self.switch_strafe_dir_count >= self.switch_strafe_dir_wait_til:
                    self.switch_strafe_dir()


    def get_new_target(self):
        candidates = []
        for x in self.game.core.world_objects:
            if x.is_ship and not x.faction is self.faction:
                candidates.append(x)

        new_target = None
        dist = None
        
        for x in candidates:
            x_dist = self.get_distance(x.x, x.y)
            if new_target is None or x_dist < dist:
                new_target = x
            dist = x_dist

        self.change_target(new_target)
        self.switch_strafe_dir()

        
    def switch_strafe_dir(self):
        self.strafe_dir = False if random.random() > .5 else True
        self.switch_strafe_dir_count = 0
        self.switch_strafe_dir_wait_til = random.randrange(50, 200)



class Asteroid(Physical_object):
    
    def __init__(self, game, parent = None):
        Physical_object.__init__(self)
        self.game = game
        self.image = self.game.core.media.gfx['world_objects_asteroid1']
        self.rotation = random.randrange(0, 360)
        self.asteroid_size = parent.asteroid_size - 1 if not parent is None else random.randrange(1, 5)
        self.custom_scale = 0.25 * self.asteroid_size

        self.collidable = True
        self.collision_type = COLLISION_TYPE_CIRCLE
        self.target_image_prefix = "asteroid"
        self.object_name = "Asteroid"
        self.targetable = True
        self.mass = 120
        self.show_on_minimap = True
        
        # Chosen to look right.
        self.moment_of_inertia = 200 * self.custom_scale
        
        self.max_health = ASTEROID_HEALTH * self.asteroid_size
        self.init()
        if parent is None:
            self.x = random.randrange(40000, 60000)
            self.y = random.randrange(40000, 60000)
        else:
            self.bump(Vector2d(math.radians(self.rotation), .5, True))
            self.x = parent.x + random.randrange(-25, 25)
            self.y = parent.y + random.randrange(-25, 25)
                    

    def register_hit(self, weapon_hit_by):
        return
        self.health -= weapon_hit_by.details['base_damage']

        # Push asteroid in correct direction
        angle = MyrmidonGame.angle_between_points((weapon_hit_by.x, weapon_hit_by.y), (self.x, self.y))
        theta = MyrmidonGame.angle_difference(angle, weapon_hit_by.rotation)
        self.bump(Vector2d(dir =  math.radians(angle), mag = (weapon_hit_by.details['speed'] * math.cos(math.radians(theta))) / (self.custom_scale * self.mass)))
    
        # add rotational velocity
        # Removed radius term because lever rotation looked awkward
        self.rotation_velocity -= (weapon_hit_by.details['speed'] * math.sin(math.radians(theta))) / self.moment_of_inertia

        """
        for x in range(3):
            Small_object_debris(self.game, 'asteroid_debris', weapon_hit_by.world_collision_point)
            """
        

    def destroy(self):
        return
        # Explosion effect
        Explosion(self.game, self, self.custom_scale)

        deb_pos = (self.x + random.randrange(int(-(self.image.width/3)*self.custom_scale), int((self.image.width/3)*self.custom_scale)),
                   self.y + random.randrange(int(-(self.image.width/3)*self.custom_scale), int((self.image.width/3)*self.custom_scale)))

        """
        for x in range(3):            
            Large_object_debris(self.game, 'debris_large', self.custom_scale * .75, deb_pos)
           """ 
        self.game.particle_emitters['debris_asteroid'].add_point(
            (self.x, self.y),
            death_timer = 2
            )

        # Make tiny asteroids
        if self.asteroid_size > 1:
            for x in range(0, 2):
                Asteroid(self.game, self)

        # Remove from world
        self.kill()



#######################################
##### WEAPONS, LASERS, MISSLES ########
#######################################



class Primary_weapon(World_object):
    
    distance_travelled = 0
    die = False
    
    def __init__(self, game, ship, mount, weapon_type):
        World_object.__init__(self)
        self.game = game
        self.ship = ship
        self.faction = self.ship.faction
        self.weapon_type = weapon_type
        self.details = WEAPON_STATS[self.weapon_type]
        #self.collision_point = self.details['collision_point']
        self.collidable = True
        self.collision_type = COLLISION_TYPE_POINT
        self.init()

        self.rotation = self.ship.rotation
        point = self.game.core.rotate_point(float(mount[0]), float(mount[1]), float(self.rotation))
        self.x = self.ship.x + point[0]
        self.y = self.ship.y + point[1]
        self.z = Z_PRIMARY_WEAPONS

        self.image = self.game.core.media.gfx[str('weapons_' +self.details['image'])]
        self.blend = True
        self.alpha = 0.0
        self.colour = self.details['colour']
        self.iter = 0
        
        
    def Execute(self):
        self.move_forward(self.details['speed'], self.rotation)

        # Collide with objects and hit them
        """
        collision = self.check_collision_with(self.game.world_objects)
        if collision[0]:
            collision[1].register_hit(self)
            self.game.particle_emitters['weapon_emitter_'+str(self.weapon_type)].add_point(
                (self.x, self.y),
                death_timer = 2
                )
            self.kill()
            return
        """
        if self.die:
            if self.image_sequence == 1:
                self.alpha -= 0.1
                if self.alpha <= 0.0:
                    self.Kill()
                    return
            else:
                if self.iter < 20:
                    self.iter += 1
                    self.custom_scale = lerp(self.iter, 20, 1.0, 0.1)
                    self.alpha = lerp(self.iter, 20, .9, .2)
                else:
                    self.Kill()
                    return
        else:
            self.distance_travelled += self.details['speed']
            if self.distance_travelled > self.details['range']:
                self.die = True
                if self.image.num_of_frames > 1:
                    self.image_sequence = 2

            if self.alpha < .9:
                self.alpha += .1




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
        self.objs = {}
        for obj in self.game.galaxy.current_solar_system.background_objects:
            self.add_new_background_object(obj)
            
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


    def add_new_background_object(self, obj):
        new_obj = Background_object(self.game, obj)
        self.objs[obj['uuid']] = new_obj
        if self.game.paused:
            new_obj.Stop_logic()
            new_obj.Stop_drawing()

        
    def move_background_object(self, obj_uuid, pos):
        self.objs[obj_uuid].x = pos[0]
        self.objs[obj_uuid].y = pos[1]
        if not self.objs[obj_uuid].child is None:
            self.objs[obj_uuid].child.x = pos[0]
            self.objs[obj_uuid].child.y = pos[1]
        

    def remove_background_object(self, obj_uuid):
        self.objs[obj_uuid].Kill()
        del(self.objs[obj_uuid])


    def change_background_object_scale(self, obj_uuid, scale):
        self.objs[obj_uuid].scale = scale
        if not self.objs[obj_uuid].child is None:
            self.objs[obj_uuid].child.scale = scale


    def change_background_object_type(self, obj_uuid, type):
        self.objs[obj_uuid].set_type(type)


    def On_exit(self):
        self.space_dust.Kill()
        for x in self.objs:
            self.objs[x].Kill()



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

        #self.draw_strategy = "space_dust"



class Background_object(Process):
    child = None
    def __init__(self, game, obj_info):
        Process.__init__(self)
        self.game = game
        self.x = obj_info['x']
        self.y = obj_info['y']
        self.z = Z_BACKGROUND_OBJECT
        self.obj_info = obj_info
        self.set_type(self.obj_info['type'])
        self.priority = PRIORITY_WORLD_OBJECTS
        self.scale = self.obj_info['scale']


    def Execute(self):
        # Recreate the child if we don't have one but need it
        if self.child is None and self.obj_type[:4] == "star":
            self.child = Background_object_sun_over(self.game, self.obj_info)
            self.blend = True
        else:
            self.blend = False


    def set_type(self, type):
        if not self.child is None:
            self.child.Kill()
            self.child = None

        self.obj_type = type
        self.image = self.game.core.media.gfx[str("background_objects_" + self.obj_type)]

        
    def On_exit(self):
        if not self.child is None:
            self.child.Kill()
        

    def get_screen_draw_position(self):
        world_pos = (
            ((float(self.x) - (self.game.camera.x)) / 30.0) + ((self.game.settings['screen_width']/2)),
            ((float(self.y) - (self.game.camera.y)) / 30.0) + ((self.game.settings['screen_height']/2))
            )
        return (world_pos[0] - ((self.image.width/2) * self.scale), world_pos[1] - ((self.image.height/2) * self.scale))

            
            
class Background_object_sun_over(Background_object):
    def __init__(self, game, obj_info):
        Process.__init__(self)
        self.game = game
        self.x = obj_info['x']
        self.y = obj_info['y']
        self.z = Z_BACKGROUND_OBJECT_SUN_OVER
        self.image = self.game.core.media.gfx[str("background_objects_" + obj_info['type'] + "_over")]
        self.priority = PRIORITY_WORLD_OBJECTS
        self.scale = obj_info['scale'] - .5
        self.alpha = .5
        self.blend = True


    def Execute(self):
        pass
