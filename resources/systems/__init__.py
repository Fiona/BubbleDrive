"""
------------------------------------------------
Space game
(c) Stompy Blondie 2011 http://stompyblondie.com
------------------------------------------------
This is the file containing the template interface for solar system scripts.
It can be considered a crappy bit of documentation for writing your own scripts.
"""


class Solar_system_script(object):

    def __init__(self, game, solar_system):
        """
        This should not be changed. It gives the script pointer to the main game object
        allowing scripts to access everything in the game.
        It also assigns the relevant Solar_system object for this script for easy access.
        """
        self.game = game
        self.solar_system = solar_system

        
    def after_load_solar_system(self):
        """
        Called after the solar system is loaded and the internal object is created.
        This is done after starting a new game or loading a game.
        Use this to make any changes to the internal object that need to be code driven.
        """
        pass
