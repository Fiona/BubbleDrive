"""
Bubble Drive
Action space RPG
(c) Stompy Blondie 2011/2012 http://stompyblondie.com
"""

# System and Python lib imports
import os, sys, uuid, glob, jsonpickle 

# Game engine imports
from core import *

# Game imports
from consts import *



class Galaxy(Process):

    # Dictionary of solar systems, short names pointing to solar system objects
    solar_systems = {}
    
    # Pointer to the current system
    current_solar_system = None
    
    def __init__(self, game):
        Process.__init__(self)
        self.game = game
        self.load_in_solar_systems()
        self.current_solar_system = self.solar_systems[STARTING_SOLAR_SYSTEM]


    def load_in_solar_systems(self):
        # Get list of relevant json files
        json_files = glob.glob(os.path.join("systems", "system_*.json"))

        # Create objects for each one
        for json_file_name in json_files:
            f = open(json_file_name, "r")
            system = jsonpickle.decode(f.read())
            f.close()

            self.solar_systems[system.short_name] = system
        

    # ####################################################
    # SOLAR SYSTEM EDITING
    # ####################################################


    def add_new_solar_system(self, short_name, galaxy_pos, full_name = "New Solar System"):
        if short_name == "":
            raise Exception("Short name required.")
        if full_name == "":
            raise Exception("Full name required.")
        
        try:
            new_system = Solar_system(galaxy_pos, short_name, full_name)
            f = open(os.path.join("systems", "system_" + new_system.short_name + ".json"), "w+")
            f.write(jsonpickle.encode(new_system))
            f.close()
        
            f = open(os.path.join("systems", "system_" + new_system.short_name + ".py"), "w+")
            f.write("# Solar system script for " + short_name + "\n\nclass " + short_name + "_solar_system_script(Solar_system_script):\n\tpass")
            f.close()
            
        except IOError:
            raise

        self.solar_systems[short_name] = new_system        
        return new_system
        

    def move_solar_system(self, short_name, pos):
        self.solar_systems[short_name].galaxy_pos = pos
        try:
            self.write_solar_system_json_file(short_name)
        except IOError:
            raise


    def remove_solar_system(self, short_name):
        try:
            os.remove(os.path.join("systems", "system_" + short_name + ".py"))
            os.remove(os.path.join("systems", "system_" + short_name + ".json"))
        except EnvironmentError:
            raise

        del(self.solar_systems[short_name])
        

    def rename_solar_system(self, short_name, new_full_name):
        self.solar_systems[short_name].name = new_full_name
        try:
            self.write_solar_system_json_file(short_name)
        except IOError:
            raise


    def recolour_solar_system(self, short_name, colour):
        self.solar_systems[short_name].colour = colour
        try:
            self.write_solar_system_json_file(short_name)
        except IOError:
            raise


    def write_solar_system_json_file(self, short_name):
        try:
            f = open(os.path.join("systems", "system_" + short_name + ".json"), "w+")
            f.write(jsonpickle.encode(self.solar_systems[short_name]))
            f.close()
        except IOError:
            raise
        

    # ####################################################
    # BACKGROUND OBJECT EDITING
    # ####################################################


    def add_new_background_object(self, pos, type):
        new_obj = {'uuid' : str(uuid.uuid4()), 'type' : type, 'x' : pos[0], 'y' : pos[1], 'scale' : .99}
        self.current_solar_system.background_objects.append(new_obj)
        self.game.background.add_new_background_object(new_obj)
        try:
            self.write_solar_system_json_file(self.current_solar_system.short_name)
        except IOError:
            raise        
        return new_obj


    def move_background_object(self, object_uuid, pos):
        obj = [x for x in self.current_solar_system.background_objects if x['uuid'] == object_uuid][0]
        obj['x'] = pos[0]
        obj['y'] = pos[1]
        self.game.background.move_background_object(object_uuid, pos)
        try:
            self.write_solar_system_json_file(self.current_solar_system.short_name)
        except IOError:
            raise        


    def remove_background_object(self, object_uuid):
        obj = [x for x in self.current_solar_system.background_objects if x['uuid'] == object_uuid][0]
        self.current_solar_system.background_objects.remove(obj)
        self.game.background.remove_background_object(object_uuid)
        try:
            self.write_solar_system_json_file(self.current_solar_system.short_name)
        except IOError:
            raise        
        

    def change_background_object_scale(self, object_uuid, scale):
        obj = [x for x in self.current_solar_system.background_objects if x['uuid'] == object_uuid][0]
        obj['scale'] = scale
        self.game.background.change_background_object_scale(object_uuid, scale)
        try:
            self.write_solar_system_json_file(self.current_solar_system.short_name)
        except IOError:
            raise        


    def change_background_object_type(self, object_uuid, type):
        obj = [x for x in self.current_solar_system.background_objects if x['uuid'] == object_uuid][0]
        obj['type'] = type
        self.game.background.change_background_object_type(object_uuid, type)
        try:
            self.write_solar_system_json_file(self.current_solar_system.short_name)
        except IOError:
            raise        



class Solar_system(object):
    def __init__(self, galaxy_pos, short_name, full_name):
        self.galaxy_pos = galaxy_pos
        self.short_name = short_name
        self.name = full_name
        self.background_seed = 0        
        self.background_objects = [
            {'uuid' : "1", 'type' : 'star1', 'x' : SYSTEM_SIZE/2, 'y' : SYSTEM_SIZE/2, 'scale' : .5},
            ]
        self.colour = "white"
