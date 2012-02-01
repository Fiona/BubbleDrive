"""
Bubble Drive
Action space RPG
(c) Stompy Blondie 2011/2012 http://stompyblondie.com
"""

# python imports
import sys, os

# Game engine imports
from core import *

# Game imports
sys.path.append(os.path.join(os.getcwd(), "logic"))
from consts import *
from gui import GUI
from galaxy import Galaxy
from game_objects import Test
#from game_objects import Camera, Player_ship, Background, Background_object, Asteroid, Mouse_object, AI_ship
from game_objects import Background
#from particles import ParticleSystem


class Game(Process):
    """
    Main game process.
    """
    # Major game settings like screen size and audio volume
    # If you change settings you need to call core.settings.save() to make sure they're updated.
    settings = {
        'screen_width' : 1,
        'screen_height' : 1,
        'full_screen' : 1,
        
        'key_ship_forward' : 1,
        'key_ship_back' : 1,
        'key_ship_left' : 1,
        'key_ship_right' : 1,
        'key_zoom_in' : 1,
        'key_zoom_out' : 1
        }

    # Value between 0 an 1 representing the current zoom level
    global_scale = 0.5

    # Current state
    game_state = 0

    # if we're paused or not
    paused = False

    # if we want the in-game editor to be enabled. Pass --editor to the program to enable.
    editor_enabled = False

    # Self explanitory object pointers and lists
    fps_text = None
    num_process_text = None


    def __init__(self, core):
        Process.__init__(self)
        self.core = core
        self.Init()
        

    def Init(self):
        """
        Init method for the main game.
        Stores common settings.
        """
        self.settings['screen_width'] = self.core.settings.screen_width
        self.settings['screen_height'] = self.core.settings.screen_height
        self.settings['full_screen'] = self.core.settings.full_screen
        self.settings['key_ship_forward'] = self.core.settings.key_ship_forward

        # Debug display
        if DEBUG_SHOW_FPS:
            self.fps_text = Text(self.core.media.fonts["basic"], 0, 0, TEXT_ALIGN_TOP_LEFT, "blank")
            self.fps_text.colour = (1.0, 1.0, 1.0)
            self.fps_text.z = -2000

        if DEBUG_SHOW_NUM_PROCESSES:
            self.num_process_text = Text(self.core.media.fonts["basic"], 0, 15.0, TEXT_ALIGN_TOP_LEFT, "blank")
            self.num_process_text.colour = (1.0, 1.0, 1.0)
            self.num_process_text.z = -2000
            
        self.gui = GUI(self)
        self.switch_game_state_to(GAME_STATE_LOGO)


    def Execute(self):
        """
        Execute method for the main game
        updates debug display
        """
        if DEBUG_SHOW_FPS:
            self.fps_text.text = "fps: " + str(self.core.current_fps)

        if DEBUG_SHOW_NUM_PROCESSES:
            self.num_process_text.text = "pro: " + str(self.core.process_count)


    def quit_game(self):
        """
        Immediately closes the game.
        """
        self.core.Quit()


    def switch_game_state_to(self, state, gui_state = None):
        """
        Pass in a state and this will switch to it.
        It will also clean up everying necessary to go out of the
        previous game state.
        """
        # Undo and destroy everything in the current state
        if self.game_state == GAME_STATE_LOGO:
            self.gui.destroy_current_gui_state()
        elif self.game_state == GAME_STATE_IN_GAME:
            self.galaxy.Kill()
            self.galaxy = None
            #self.camera.signal(S_KILL)
            #self.camera = None
            self.background.Kill()
            self.background = None
            #self.player_ship = None

            #for x in self.world_objects:
            #    self.world_objects[x].kill()
            #self.world_objects = []

            self.world_objects_by_faction = {
                FACTION_NEUTRAL : [],
                FACTION_PLAYER : []
                }

        # Switch to new state
        self.game_state = state

        # Create everything we require
        if state == GAME_STATE_LOGO:
            self.gui.switch_gui_state_to(GUI_STATE_LOGO if gui_state is None else gui_state)

        elif state == GAME_STATE_IN_GAME:            
            self.galaxy = Galaxy(self)
            Test(self)
            #self.camera = Camera(self)
            
            #self.particle_emitters = {}
            #self.particles = ParticleSystem(self, Z_PARTICLES)
            #self.create_emitters()
                    
            #self.player_ship = Player_ship(self)
            #self.camera.x, self.camera.y = self.player_ship.x, self.player_ship.y
            
            self.background = Background(self)
                            
            #for x in range(50):
            #    Asteroid(self)
            #AI_ship(self, {'type' : SHIP_TYPE_PROSPERO_FIGHTER, 'x' : 50500, 'y' : 50500})
 
            #self.camera.set_anchor_to(self.player_ship, instant = True)

            self.gui.fade_toggle()
            self.gui.switch_gui_state_to(GUI_STATE_WORLD_VIEW)


    def game_event(self, event_code, param_1 = None):
        """
        I'll write this docstring later
        """
        if event_code == EVENT_WORLD_OBJECT_DIED:
            ship = param_1
            if self.player_ship.target == ship:
                self.player_ship.change_target(None)


    def pause_game(self):
        if self.paused:

            return

        for x in self.world_objects:
            x.Stop_logic(tree = True)
            x.Stop_drawing(tree = True)

        self.particles.Stop_logic(tree = True)
        self.background.Stop_logic(tree = True)
        self.gui.pause_game()

        self.paused = True
                

    def unpause_game(self):
        if not self.paused:
            return

        for x in self.world_objects:
            x.Start_logic(tree = True)
            x.Start_drawing(tree = True)

        self.particles.Start_logic(tree = True)
        self.background.Start_logic(tree = True)
        self.gui.unpause_game()

        self.paused = False


Game(core)
