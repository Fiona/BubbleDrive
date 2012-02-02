"""
Space game
(c) Stompy Blondie 2010 http://stompyblondie.com
"""
from core import *

# Debug values
DEBUG_SHOW_FPS = True
DEBUG_SHOW_NUM_PROCESSES = True
DEBUG_SHOW_COLLISION_BOXES = False

# States
GAME_STATE_LOGO = 1
GAME_STATE_IN_GAME = 2

# GUI states
GUI_STATE_LOGO = 1
GUI_STATE_WORLD_VIEW = 2
GUI_STATE_IN_GAME_MENU = 3

GUI_IN_GAME_MENU_MODE_SYSTEM_VIEW = 1
GUI_IN_GAME_MENU_MODE_GALAXY_VIEW = 2

# Used for reporting events to the main game that need propagate around
EVENT_WORLD_OBJECT_DIED = 1

# Keeps track of what we're doing when bubbling
BUBBLE_JUMP_PHASE_ENTERING_BUBBLE = 1
BUBBLE_JUMP_PHASE_TRAVELLING_INTRASYSTEM = 2
BUBBLE_JUMP_PHASE_LEAVING_BUBBLE = 3
BUBBLE_JUMP_PHASE_TRAVELLING_INTERSYSTEM = 4

# Factions
FACTION_MINIMAP_BLIP_COLOURS = {
    FACTION_NEUTRAL : (.5, .5, 1.0),
    FACTION_PLAYER : (0, 1, 0),
    FACTION_OTHER : (.5, 1.0, 1.0),
    FACTION_ENEMY : (1.0, 0, 0)
    }

# Weapon related
WEAPON_CLASS_LASER = 1

WEAPON_TYPE_LASER_1 = 1

WEAPON_STATS = {
    WEAPON_TYPE_LASER_1 : {
        'class' : WEAPON_CLASS_LASER,
        'image' : 'laser1',
        'speed' : 12.0,
        'colour' : (.5, 0.0, .5),
        'range' : 600,
        'reload_time' : 60,
        'collision_point' : (8, 0),
        'base_damage' : 10,
        }
    }


# Ship related constants
SHIP_TYPE_TEST_FIGHTER = 1
SHIP_TYPE_PROSPERO_FIGHTER = 2

SHIP_STATS = {
    SHIP_TYPE_TEST_FIGHTER : {
        'name' : "Test",
        'health' : 200,
        'shields' : 200,

        'weapon_mounts' : [
            {'class' : WEAPON_CLASS_LASER, 'default' : WEAPON_TYPE_LASER_1, 'point' : (6, 9)},
            {'class' : WEAPON_CLASS_LASER, 'default' : WEAPON_TYPE_LASER_1, 'point' : (6, -9)}
            ],

        'acceleration' : 0.005,
        'max_acceleration' : 0.1,
        'max_speed' : 1.0,
        'turn_rate' : 4,
        
        'width' : 64,
        'height' : 52,

        'engine_glow' : [
                {
                    'type' : 1,
                    'point' : (-42, 0),
                    'colour' : (1.0, 1.0, .5)
                }
            ],        
        
        'glow_map_colour' : (1.0, .8, 1.0)
        },
    
    SHIP_TYPE_PROSPERO_FIGHTER : {
        'name' : "Prospero Class",
        'health' : 200,
        'shields' : 200,

        'weapon_mounts' : [
            {'class' : WEAPON_CLASS_LASER, 'default' : WEAPON_TYPE_LASER_1, 'point' : (0, 14)},
            {'class' : WEAPON_CLASS_LASER, 'default' : WEAPON_TYPE_LASER_1, 'point' : (0, -14)}
            ],

        #'acceleration' : 1.0,
        'acceleration' : 0.005,
        'max_acceleration' : 0.1,
        #'max_acceleration' : 10.0,
        'max_speed' : 1.0,
        'turn_rate' : 2,
        
        'width' : 60,
        'height' : 50,

        'mass' : 100,
        'explosion_size' : .5,

        'bubble_jump_size' : .5,
        
        'engine_glow' : [
                {
                    'dir' : 'forward',    
                    'type' : 1,
                    'point' : (-20, 0),
                    'colour' : (1.0, 1.0, 1.0),
                    'rotation' : 0,
                    'scale' : 1.0
                },
                {
                    'dir' : 'forward',    
                    'type' : 1,
                    'point' : (-22, -2),
                    'colour' : (1.0, 1.0, 1.0),
                    'rotation' : 0,
                    'scale' : 1.0
                },
                {
                    'dir' : 'forward',    
                    'type' : 1,
                    'point' : (-22, 2),
                    'colour' : (1.0, 1.0, 1.0),
                    'rotation' : 0,
                    'scale' : 1.0
                },
                
                {
                    'dir' : 'backward',
                    'type' : 1,
                    'point' : (0, 14),
                    'colour' : (1.0, 1.0, 1.0),
                    'rotation' : 180,
                    'scale' : .5
                },
                {
                    'dir' : 'backward',
                    'type' : 1,
                    'point' : (0, -14),
                    'colour' : (1.0, 1.0, 1.0),
                    'rotation' : 180,
                    'scale' : .5
                },

                {
                    'dir' : 'right',
                    'type' : 1,
                    'point' : (-24, 15),
                    'colour' : (1.0, 1.0, 1.0),
                    'rotation' : 90,
                    'scale' : .5
                },
                {
                    'dir' : 'left',
                    'type' : 1,
                    'point' : (-24, -15),
                    'colour' : (1.0, 1.0, 1.0),
                    'rotation' : 270,
                    'scale' : .5
                },
                
            ],

        'glow_map_colour' : (1.0, .8, 1.0)
        }
    
    }

# Misc settings
SOLAR_SYSTEM_COLOURS = {
    "white" : (1.0, 1.0, 1.0),
    "yellow" : (1.0, 1.0, 0.8),
    "red" : (1.0, 0.8, 1.0),
    "green" : (0.8, 1.0, 0.8),
    "blue" : (0.8, 0.8, 1.0)
}    
WORLD_GRID_WIDTH = 500
WORLD_GRID_HEIGHT = 500
MIN_ZOOM_LEVEL = 0.4
SHAKE_CAM_TIME = 20
MOVE_CAMERA_TIME = 10
SPACE_DUST_NUM = 20
SPACE_DUST_TYPES = 4
ASTEROID_HEALTH = 50
MINIMAP_RANGE = 3000
TOOLTIP_WAIT_TIME = 10
BUBBLE_JUMP_INTRASYSTEM_SPEED = 50
MINIMUM_BUBBLE_JUMP_DISTANCE = 5000
STARTING_SOLAR_SYSTEM = "adrexis"

# List of background items (corresponds to image names)
BACKGROUND_OBJECT_TYPES = ['star1', 'planet1', 'planet_broken1']

# IMPORTANT NEVER CHANGE THESE SETTINGS FOR GOD SAKE
SYSTEM_SIZE = 100000

# Z vals
Z_STOMPYBLONDIE_LOGO = 1000
Z_BACKGROUND = 400
Z_BACKGROUND_OBJECT = 350
Z_SPACE_DUST = 300
Z_SHIP_DEBRIS = -150
Z_PRIMARY_WEAPONS = -250
Z_SHIPS = -300
Z_PARTICLES = -350
Z_EXPLOSIONS = -400
Z_BACKGROUND_OBJECT_SUN_OVER = -460
Z_WORLD_LEVEL_GUI = -475
Z_GUI = -500
Z_GUI_CONTAINERS = -550
Z_GUI_OBJECT_LEVEL_1 = -560
Z_GUI_OBJECT_LEVEL_2 = -570
Z_GUI_OBJECT_LEVEL_3 = -580
Z_GUI_OBJECT_LEVEL_4 = -590
Z_GUI_OBJECT_LEVEL_5 = -600
Z_GUI_OBJECT_LEVEL_6 = -610
Z_GUI_OBJECT_LEVEL_7 = -620
Z_GUI_OBJECT_LEVEL_8 = -630
Z_GUI_OBJECT_LEVEL_9 = -640
Z_GUI_OBJECT_LEVEL_10 = -650
Z_MOUSE = -1000
Z_GUI_TOOLTIP = -1500

# Process priorities
PRIORITY_MAIN_GAME = 150
PRIORITY_GUI_ELEMENTS = 500
PRIORITY_GUI = 750
PRIORITY_CAMERA = 800
PRIORITY_WORLD_OBJECTS = 900
PRIORITY_GUI_MINIMAP = 1000
