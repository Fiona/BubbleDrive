/****************************
 BUBBLE DRIVE
 2011/2012 STOMPY BLONDIE GAMES
 ****************************
 World and game objects source file
 ****************************/


#include "main.h"


/**
 * World objects are physical objects that will also take
 * care of position in the world relative to the camera.
 */
World_object::World_object(): Process()
{
    custom_scale = 1.0f;
    recalculate_corner_vectors = True;
    corner_vectors.resize(4);
    corner_vectors[CORNER_UL].resize(2, 0.0f);
    corner_vectors[CORNER_UR].resize(2, 0.0f);
    corner_vectors[CORNER_LL].resize(2, 0.0f);
    corner_vectors[CORNER_LR].resize(2, 0.0f);
    collidable = False;
    collision_type = COLLISION_TYPE_RECTANGLE;
    collision_rectangle_radius = 0.0f;
    width = -1;
    height = -1;
    radius = -1;
    targetable = False;
    target_image_prefix = "";
    show_on_minimap = False;
    is_ship = False;
    object_name = "";
    faction = FACTION_NEUTRAL;
    max_health = 100;
    health = 100;
    max_shields = -1;
    shields = -1;
}


void World_object::init()
{

    priority = PRIORITY_WORLD_OBJECTS;
    recalculate_corner_vectors = True;

    Main_App* core = Main_App::Instance();

    if(targetable)
        core->targetable_world_objects.push_back(this);

    core->world_objects.push_back(this);
    core->world_objects_by_faction[faction].push_back(this);
    health = max_health;

}


void World_object::Kill()
{

    vector<Process*>::iterator it;
    Main_App* core = Main_App::Instance();

    it = std::find(core->world_objects.begin(), core->world_objects.end(), this);
    if(it != core->world_objects.end())
        it = core->world_objects.erase(it);

    it = std::find(core->world_objects_by_faction[faction].begin(), core->world_objects_by_faction[faction].end(), this);
    if(it != core->world_objects_by_faction[faction].end())
        it = core->world_objects_by_faction[faction].erase(it);

    if(targetable)
    {
        it = std::find(core->targetable_world_objects.begin(), core->targetable_world_objects.end(), this);
        if(it != core->targetable_world_objects.end())
            it = core->targetable_world_objects.erase(it);
    }

}


void World_object::Execute()
{

    Main_App* core = Main_App::Instance();

    scale = core->global_scale * custom_scale;

    if(health <= 0)
        Destroy();

    if(show_on_minimap)
        if(
            x > core->camera_x - MINIMAP_RANGE &&
            x < core->camera_x + MINIMAP_RANGE &&
            y > core->camera_y - MINIMAP_RANGE &&
            y < core->camera_y + MINIMAP_RANGE
            )
            core->minimap_objects.push_back(this);

}


void World_object::Destroy()
{
    World_object::Kill();
}


void World_object::set_rotation(int rotation_)
{
    if(rotation != rotation_)
        recalculate_corner_vectors = True;
    rotation = rotation_;    
}


void World_object::set_x(float x_)
{
    if(x != x_)
        recalculate_corner_vectors = True;
    x = x_;    
}


void World_object::set_y(float y_)
{
    if(y != y_)
        recalculate_corner_vectors = True;
    y = y_;    
}


tuple<float, float> World_object::get_screen_draw_position()
{
    if(image == NULL)
        return tuple<float, float>(x, y);

    return tuple<float, float>(x - (image -> width / 2), y - (image -> height / 2));
}


/**
   Required for computing rectangle collisions. This works out where in the world
   the four corners of the bounding box are.
*/
void World_object::calculate_corner_vectors()
{

    if(recalculate_corner_vectors == False)
        return;
        
    if(width == -1)
        width = image->width * custom_scale;
    if(height == -1)
        height = image->height * custom_scale;

    tuple<float, float> rot;

    rot = Main_App::rotate_point((float)(-width/2), (float)(-height/2), (float)rotation);
    corner_vectors[CORNER_UL][0] = x + rot.get<0>();
    corner_vectors[CORNER_UL][1] = y + rot.get<1>();

    rot = Main_App::rotate_point((float)(width/2), (float)(-height/2), (float)rotation);
    corner_vectors[CORNER_UR][0] = x + rot.get<0>();
    corner_vectors[CORNER_UR][1] = y + rot.get<1>();

    rot = Main_App::rotate_point((float)(-width/2), (float)(height/2), (float)rotation);
    corner_vectors[CORNER_LL][0] = x + rot.get<0>();
    corner_vectors[CORNER_LL][1] = y + rot.get<1>();

    rot = Main_App::rotate_point((float)(width/2), (float)(height/2), (float)rotation);
    corner_vectors[CORNER_LR][0] = x + rot.get<0>();
    corner_vectors[CORNER_LR][1] = y + rot.get<1>();

    recalculate_corner_vectors = False;

}


/*
 * World object stuff for the python wrapper
 */
World_objectWrapper::World_objectWrapper() : World_object(){}
World_objectWrapper::World_objectWrapper(PyObject* _self) : World_object()
{
    self = _self;
    self_ = boost::python::object(boost::python::handle<>(boost::python::borrowed(self)));
    Process::internal_list.append(self_);
    this->Process::self = self;
    this->Process::self_ = self_;
}


void World_objectWrapper::Execute()
{
    boost::python::call_method<void>(self, "Execute");
    if(!is_dead)
        this->World_object::Execute();
}
void World_objectWrapper::Execute_default()
{
    this->World_object::Execute();
}


void World_objectWrapper::Destroy()
{
    boost::python::call_method<void>(self, "Destroy");
    this->World_object::Destroy();
}
void World_objectWrapper::Destroy_default()
{
    this->World_object::Destroy();
}


tuple<float, float> World_objectWrapper::get_screen_draw_position()
{
    boost::python::object tup = boost::python::call_method<boost::python::object>(self, "get_screen_draw_position");
    return tuple<float, float>(boost::python::extract<float>(tup[0]), boost::python::extract<float>(tup[1]));
}
tuple<float, float> World_objectWrapper::get_screen_draw_position_default()
{
    return this->World_object::get_screen_draw_position();
}


void World_objectWrapper::Kill()
{
    boost::python::call_method<void>(self, "On_Exit");
    Process::internal_list.remove(self_);
    this->World_object::Kill();
    boost::python::decref(self);
    boost::python::decref(self);
    self = NULL;
}
