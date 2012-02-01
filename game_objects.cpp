/****************************
 BUBBLE DRIVE
 2011/2012 STOMPY BLONDIE GAMES
 ****************************
 World and game objects source file
 ****************************/


#include "main.h"


/**
 *
 */
World_object::World_object(): Process()
{
    x = y = 0.0f;
    z = 0;
    scale = 1.0;
    rotation = 0;
    alpha = 1.0;
    image_sequence = 1;
    draw_strategy = "";
    is_dead = False;
}


void World_object::Set_z(int new_z)
{
    z = new_z;
    Process::z = new_z;
    Process::z_order_dirty = True;
}


void World_object::Set_colour(boost::python::object list)
{
    colour[0] = boost::python::extract<float>(list[0]);
    colour[1] = boost::python::extract<float>(list[1]);
    colour[2] = boost::python::extract<float>(list[2]);
}


tuple<float, float> World_object::get_screen_draw_position()
{

    if(image == NULL)
        return tuple<float, float>(x, y);

    return tuple<float, float>(x - (image -> width / 2), y - (image -> height / 2));

}


void World_object::Draw()
{

    if(image == NULL || is_dead == True)
        return;

    glPushMatrix();

    // Get drawing coords
    tuple<float, float> draw_pos = get_screen_draw_position();

    // glrotate works by you translating to the point around which you wish to rotate
    // and applying the rotation you can translate back to apply the real translation
    // position
    if(rotation < 0 || rotation > 0)
    {
        float rot_x = (draw_pos.get<0>() * scale) + ((image->width/2) * scale);
        float rot_y = (draw_pos.get<1>() * scale) + ((image->height/2) * scale);
        glTranslatef(rot_x, rot_y, 0.0f);
        glRotatef((float)rotation, 0.0f, 0.0f, 1.0f);
        glTranslatef(-rot_x, -rot_y, 0.0f);
    }

    // move to position
    glTranslatef(draw_pos.get<0>(), draw_pos.get<1>(), 0.0f);

    // scaling
    if(scale < 1.0f || scale > 1.0f)
    {
        glTranslatef(scale_pos[0], scale_pos[1], 0.0f);
        glScalef(scale, scale, 1.0f);
        glTranslatef(-scale_pos[0], -scale_pos[1], 0.0f);
    }

    // Text texture coords to different ones for texture atlasses
    if(image->num_of_frames > 1)
        glTexCoordPointer(2, GL_FLOAT, 0, &image->texture_coords[image_sequence-1][0]);

    // Binding texture
    if(Process::current_bound_texture != image->texture)
    {
        glBindTexture(GL_TEXTURE_2D, image->texture);
        glVertexPointer(3, GL_FLOAT, 0, image->vertex_list);
        Process::current_bound_texture = image->texture;
    }

    // Changing colour and transparency
    glColor4f(colour[0], colour[1], colour[2], alpha);

    // draw the triangle strip
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Reset the texture coords back to default if necessary
    if(image->num_of_frames > 1)
        glTexCoordPointer(2, GL_FLOAT, 0, &Process::default_texture_coords[0]);

    glPopMatrix();

}


void World_objectWrapper::Execute()
{
    boost::python::call_method<void>(self, "Execute");
}
void World_objectWrapper::Execute_default()
{
    this->World_object::Execute();
}


void World_objectWrapper::Kill()
{
    is_dead = True;
    Process::internal_list.remove(self_);
    this->Process::Kill();
    boost::python::decref(self);
    boost::python::decref(self);
    self = NULL;
}


void World_objectWrapper::On_Exit(){ }
void World_objectWrapper::On_Exit_default()
{
    this->World_object::On_Exit();
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


World_objectWrapper::World_objectWrapper(PyObject* _self) : World_object()
{
    self = _self;
    self_ = boost::python::object(boost::python::handle<>(boost::python::borrowed(self)));
    Process::internal_list.append(self_);
    this->Process::self = self;
    this->Process::self_ = self_;
}

