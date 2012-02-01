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
from consts import *
from helpers  import *
from gui.logo import *
#from gui.world_view import *
#from gui.in_game_menu import *



class Mouse(Process):
    """
    An instance is created and stored in the GUI object.
    """
    def __init__(self, game):
        Process.__init__(self)
        self.game = game
        self.z = Z_MOUSE


    def Execute(self):
        self.x = self.game.core.mouse.x
        self.y = self.game.core.mouse.y


    def get_screen_draw_position(self):
        return self.x, self.y

        

class GUI(Process):
    """ High level gui handler """
    # Current gui state
    gui_state = None
        
    # This is a dictionary of gui state names pointing to dictionaries of gui names poiting to gui elements visible in that state
    current_visible_gui_elements = {
        GUI_STATE_LOGO : {},
        GUI_STATE_WORLD_VIEW : {},
        GUI_STATE_IN_GAME_MENU : {}
        }

    # goes up every frame, resets when changing game state
    current_game_state_gui_ticks = 0

    # Points to the parent window which all GUI objects will sit in and recieve input from this, can change during game
    parent_window = None

    # Simple flag you can use to stop the player escaping for instance
    block_gui_keyboard_input = False

    # Simple flag you can use to stop any input to the gui taking place
    block_gui_mouse_input = False

    # Pointer to the mouse process
    mouse = None

    def __init__(self, game):
        Process.__init__(self)
        self.game = game
        self.alpha = 1.0
        self.fading = 1.0
        self.fade_to = 0
        self.fading_done = False
        self.z = -5000
        self.priority = PRIORITY_GUI
        self.iter = 0

        # Draw strategy data
        self.draw_strategy = "primitive_square"
        self.draw_strategy_call_parent = False
        self.primitive_square_filled = True
        self.primitive_square_width = self.game.settings['screen_width']
        self.primitive_square_height = self.game.settings['screen_height']
        self.primitive_square_x = 0.0
        self.primitive_square_y = 0.0
        self.primitive_square_colour = (0.0, 0.0, 0.0, 1.0)
        self.mouse = Mouse(self.game)


    def Execute(self):
        self.current_game_state_gui_ticks += 1

        # Handle the fading stuff
        if not self.fading == None and self.fading_done == False:
            self.iter += 1
            self.alpha = lerp(self.iter, self.fade_speed, self.fading, self.fade_to)
            if self.iter == self.fade_speed:
                self.alpha = self.fade_to
                self.fading = None
                self.fading_done = True
                self.iter = 0
                if not self.fading_callback == None:
                    self.fading_callback()

        # Colour of fade
        self.primitive_square_colour = (self.fade_colour[0], self.fade_colour[1], self.fade_colour[2], self.alpha)

        # Block input on fade
        if not self.fading == None and self.fading_done == False:
            return

        if self.game.game_state == GAME_STATE_LOGO:
            """
            LOGO SCREEN
            """
            if self.current_game_state_gui_ticks == 30:
                self.current_visible_gui_elements[GAME_STATE_LOGO]['stompyblondie_logo_text'] = Stompyblondie_Logo_Text(self.game)
                #self.game.play_sound_effect("stompyblondie")
            if self.current_game_state_gui_ticks == 120 or self.game.core.Keyboard_key_down(key.ESCAPE):
                self.fade_toggle(lambda: self.game.switch_game_state_to(GAME_STATE_IN_GAME), colour = (0,0,0))

        elif self.game.game_state == GAME_STATE_IN_GAME and self.gui_state == GUI_STATE_WORLD_VIEW:
            """
            IN-GAME WORLD VIEW
            """
            """
            if not self.game.player_ship.lock_controls:
                if not self.block_gui_keyboard_input:
                    # Quit on escape
                    if MyrmidonGame.engine['input'].keyboard_key_released(K_ESCAPE):
                        self.game.quit_game()

                    # Open menu
                    if MyrmidonGame.engine['input'].keyboard_key_released(K_SPACE):
                        self.fade_toggle(lambda: self.open_in_game_menu())

                # Scroll wheel zoom input
                if not self.game.paused:
                    if MyrmidonGame.engine['input'].mouse.wheel_down:
                        self.game.global_scale *= 0.75
                    elif MyrmidonGame.engine['input'].keyboard_key_down(self.game.settings['key_zoom_out']):
                        self.game.global_scale *= 0.95
                    if MyrmidonGame.engine['input'].mouse.wheel_up:
                        self.game.global_scale *= 1.25
                    elif MyrmidonGame.engine['input'].keyboard_key_down(self.game.settings['key_zoom_in']):
                        self.game.global_scale *= 1.05

            else:
                MyrmidonGame.engine['input'].mouse.alpha = 0
        """

            
            # adjust the zoom level if we're out of bounds
            if self.game.global_scale > 1.00:
                self.game.global_scale = 1.00
            if self.game.global_scale < MIN_ZOOM_LEVEL:
                self.game.global_scale = MIN_ZOOM_LEVEL

            """
            # Rotate the mouse cursor if hovering over a targetable object
            if self.game.mouse_object.targeting[0]:
                MyrmidonGame.engine['input'].mouse.rotation += 5
                MyrmidonGame.engine['input'].mouse.scale = 1.0
                MyrmidonGame.engine['input'].mouse.colour = (.8, .8, 1)
                if MyrmidonGame.engine['input'].mouse.right_up:
                    self.game.player_ship.change_target(self.game.mouse_object.targeting[1])
                    self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW]['current_target_info_overlay'].change_object_focus(self.game.mouse_object.targeting[1])
                    self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW]['current_target_indicator'].change_object_focus(self.game.mouse_object.targeting[1])
            else:
                MyrmidonGame.engine['input'].mouse.rotation = 0
                MyrmidonGame.engine['input'].mouse.scale = .75
                MyrmidonGame.engine['input'].mouse.colour = (1, 1, 1)
                if MyrmidonGame.engine['input'].mouse.right_up:
                    self.game.player_ship.change_target(None)
                    self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW]['current_target_info_overlay'].change_object_focus(None)
                    self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW]['current_target_indicator'].change_object_focus(None)
                    """

        elif self.gui_state == GUI_STATE_IN_GAME_MENU:
            """
            IN-GAME MENU
            """
            # Close menu
            if not self.block_gui_keyboard_input:
                if self.game.core.Keyboard_key_down(key.ESCAPE):
                    self.fade_toggle(lambda: self.close_in_game_menu())

            # Wake up player ship if jumping
            #if self.game.player_ship.bubble_jump_phase == BUBBLE_JUMP_PHASE_TRAVELLING_INTRASYSTEM:
            #    self.game.player_ship.signal(S_WAKEUP)            


        # Handle overall gui input
        if not self.parent_window is None:
            mouse_over = self.parent_window.handle_input((self.mouse.x, self.mouse.y))
            if not mouse_over is None and not self.block_gui_mouse_input:

                if not mouse_over._currently_hovered:
                    mouse_over.mouse_enter()
                    mouse_over._currently_hovered = True

                mouse_over.mouse_over()

                if self.game.core.mouse.left_down:
                    mouse_over.mouse_left_down()
                elif self.game.core.mouse.left_up:
                    mouse_over.mouse_left_up()

                if self.game.core.mouse.right_down:
                    mouse_over.mouse_right_down()
                elif self.game.core.mouse.right_up:
                    mouse_over.mouse_right_up()

                if self.game.core.mouse.middle_down:
                    mouse_over.mouse_middle_down()
                elif self.game.core.mouse.middle_up:
                    mouse_over.mouse_middle_up()

                if self.game.core.mouse.wheel_down:
                    mouse_over.mouse_wheel_down()
                elif self.game.core.mouse.wheel_up:
                    mouse_over.mouse_wheel_up()
        

    def switch_gui_state_to(self, state):
        self.destroy_current_gui_state()
        
        self.gui_state = state
        self.current_game_state_gui_ticks = 0
        self.parent_window = None
                                                  
        if self.gui_state == GUI_STATE_LOGO:
            self.mouse.alpha = 1.0
            self.mouse.z = Z_MOUSE
            self.mouse.image = None
            self.current_visible_gui_elements[GUI_STATE_LOGO]['stompyblondie_logo'] = Stompyblondie_Logo(self.game)
        elif self.gui_state == GUI_STATE_WORLD_VIEW:
            self.mouse.image = self.game.core.media.gfx['gui_crosshair']

            if len(self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW]) > 0:
                return
            """
            self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW]['player_info_overlay'] = GUI_player_info_overlay(self.game)
            self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW]['current_target_info_overlay'] = GUI_current_target_info_overlay(self.game)
            self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW]['current_target_indicator'] = GUI_current_target_indicator(self.game)
            self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW]['current_target_pointer'] = GUI_current_target_pointer(self.game)
            self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW]['minimap'] = GUI_minimap(self.game)
            """
        elif self.gui_state == GUI_STATE_IN_GAME_MENU:
            self.mouse.image = self.game.core.media.graphics['gui_cursor']
            
            #self.current_visible_gui_elements[GUI_STATE_IN_GAME_MENU]['game_menu_container'] = GUI_in_game_menu_container(self.game)
            #self.parent_window = self.current_visible_gui_elements[GUI_STATE_IN_GAME_MENU]['game_menu_container']


    def destroy_current_gui_state(self):
        if self.gui_state is None:
            return
        for x in self.current_visible_gui_elements[self.gui_state]:
            self.current_visible_gui_elements[self.gui_state][x].Kill()
        self.current_visible_gui_elements[self.gui_state] = {}


    def pause_game(self):
        for x in self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW]:
            self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW][x].Stop_logic(tree = True)
            self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW][x].Stop_drawing(tree = True)


    def unpause_game(self):
        for x in self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW]:
            self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW][x].Start_drawing(tree = True)
            self.current_visible_gui_elements[GUI_STATE_WORLD_VIEW][x].Start_logic(tree = True)


    fading = None
    fade_to = 0.0
    fade_speed = 15
    fade_colour = (0, 0, 0)
    fading_done = False
    fading_callback = None

    
    def fade_toggle(self, callback = None, speed = 15, colour = (0.0, 0.0, 0.0)):
        if not self.fading_done:
            return
        self.fade_speed = speed
        self.fading_callback = callback
        self.fading = self.alpha
        self.fade_colour = colour
        self.fade_to = 1.0 if self.fading < 1.0 else 0.0
        self.fading_done = False
