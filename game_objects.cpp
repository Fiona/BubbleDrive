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

}


void World_object::Execute()
{

}


tuple<float, float> World_object::get_screen_draw_position()
{
    if(image == NULL)
        return tuple<float, float>(x, y);

    return tuple<float, float>(x - (image -> width / 2), y - (image -> height / 2));
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
