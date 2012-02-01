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
#from gui import GUI, Mouse
#from puzzle import Puzzle_manager


class Game(Process):
    """
    Main game process.
    """
    # Major game settings like screen size and audio volume
    # If you change settings you need to call core.settings.save() to make sure they're updated.
    settings = {
        'screen_width' : 1,
        'screen_height' : 1,
        'full_screen' : 1
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

        # Debug display
        if DEBUG_SHOW_FPS:
            self.fps_text = Text(self.core.media.fonts["basic"], 0, 0, TEXT_ALIGN_TOP_LEFT, "blank")
            self.fps_text.colour = (1.0, 1.0, 1.0)
            self.fps_text.z = -2000

        if DEBUG_SHOW_NUM_PROCESSES:
            self.num_process_text = Text(self.core.media.fonts["basic"], 0, 15.0, TEXT_ALIGN_TOP_LEFT, "blank")
            self.num_process_text.colour = (1.0, 1.0, 1.0)
            self.num_process_text.z = -2000
            
        #self.gui = GUI(self)
        #self.switch_game_state_to(GAME_STATE_LOGO)


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



Game(core)
