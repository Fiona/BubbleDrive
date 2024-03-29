/****************************
 BUBBLE DRIVE
 2011/2012 STOMPY BLONDIE GAMES
 ****************************
 Processesss source file
 ****************************/


#include "main.h"
#include <iostream>
#include <math.h>

std::vector<Process*> Process::Process_List;
std::vector<Process*> Process::Priority_List;
std::vector<Process*> Process::Processes_to_kill;
boost::python::list Process::internal_list;

vector<float> Process::default_texture_coords(8);

bool Process::z_order_dirty;
bool Process::priority_order_dirty;
GLuint Process::current_bound_texture = -1;


 
Process::Process()
{
    x = y = 0.0f;
    z = 0;
    priority = 0;
    scale = 1.0;
    rotation = 0;
    colour.resize(3, 1.0f);
    scale_pos.resize(2, 0.0f);
    alpha = 1.0;
    image_sequence = 1;
    draw_strategy = "";
    is_dead = False;

    self = NULL;

    image = NULL;
    Process::z_order_dirty = True;
    Process::priority_order_dirty = True;
    Process::Priority_List.push_back(this);
    Process::Process_List.push_back(this);
}


Process::~Process()
{
}


void Process::Execute()
{
}


void Process::On_Exit()
{
}


void Process::Draw()
{

    if(image == NULL || is_dead == True || alpha <= 0.0f)
        return;

    glPushMatrix();

    // Get drawing coords
    tuple<float, float> draw_pos = get_screen_draw_position();

    // glrotate works by you translating to the point around which you wish to rotate
    // and applying the rotation you can translate back to apply the real translation
    // position
    if(rotation < 0 || rotation > 0)
    {
        //float rot_x = (draw_pos.get<0>() * scale) + ((image->width/2) * scale);
        //float rot_y = (draw_pos.get<1>() * scale) + ((image->height/2) * scale);
        float rot_x = draw_pos.get<0>() + (image->width/2) * scale;
        float rot_y = draw_pos.get<1>() + (image->height/2) * scale;
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


void Process::Kill()
{
    Process::Processes_to_kill.push_back(this);
    is_dead = True;
}


void Process::Set_z(int new_z)
{
    z = new_z;
    Process::z_order_dirty = True;
}


void Process::Set_priority(int priority_)
{
    priority = priority_;
    Process::priority_order_dirty = True;
}


void Process::Set_colour(boost::python::object list)
{
    colour[0] = boost::python::extract<float>(list[0]);
    colour[1] = boost::python::extract<float>(list[1]);
    colour[2] = boost::python::extract<float>(list[2]);
}


void Process::Set_scale_pos(boost::python::object list)
{
    scale_pos[0] = boost::python::extract<float>(list[0]);
    scale_pos[1] = boost::python::extract<float>(list[1]);
}


void Process::move_forward(float distance_to_travel, int rotation_to_move_in)
{
    x = x + distance_to_travel * cos(deg_to_rad((float)rotation_to_move_in));
    y = y + distance_to_travel * sin(deg_to_rad((float)rotation_to_move_in));
}


float Process::get_distance(float x_, float y_)
{
    return sqrt(
        (pow((y_ - y), 2) + pow((x_ - x), 2))
        );
}


tuple<float, float> Process::get_screen_draw_position()
{

    if(image == NULL)
        return tuple<float, float>(x, y);

    return tuple<float, float>(x - (image -> width / 2), y - (image -> height / 2));

}



/*
 *
 */
ProcessWrapper::ProcessWrapper(PyObject* _self) : Process()
{
    has_init = False;
    has_killed = False;
    self = _self;
    self_ = boost::python::object(boost::python::handle<>(boost::python::borrowed(self)));
    Process::internal_list.append(self_);

    this->Process::self = self;
    this->Process::self_ = self_;
}


void ProcessWrapper::Kill()
{
    boost::python::call_method<void>(self, "On_Exit");
    Process::internal_list.remove(self_);
    this->Process::Kill();
    boost::python::decref(self);
    boost::python::decref(self);
    self = NULL;
    is_dead = True;
    this->Process::is_dead = True;
}



void ProcessWrapper::Execute()
{
    boost::python::call_method<void>(self, "Execute");
}
void ProcessWrapper::Execute_default()
{
    this->Process::Execute();
}


void ProcessWrapper::On_Exit(){ }
void ProcessWrapper::On_Exit_default()
{
    this->Process::On_Exit();
}


tuple<float, float> ProcessWrapper::get_screen_draw_position()
{
    boost::python::object tup = boost::python::call_method<boost::python::object>(self, "get_screen_draw_position");
    return tuple<float, float>(boost::python::extract<float>(tup[0]), boost::python::extract<float>(tup[1]));
}
tuple<float, float> ProcessWrapper::get_screen_draw_position_default()
{
    return this->Process::get_screen_draw_position();
}

/*
 *
 */
Text::Text(): Process()
{
    font = NULL;
    alignment = 0;
    text_width = 0;
    text_height = 0;
    text = "";
}


Text::Text(Font* _font, float _x, float _y, int _alignment, string _text): Process()
{

    font = _font;
    x = _x;
    y = _y;
    z = Z_TEXT;
    priority = 0;
    Process::z = z;
    alignment = _alignment;
    shadow = 0;
    shadow_colour.resize(3, 1.0f);
    text_width = 0;
    text_height = 0;
    image_sequence = 1;
    scale = 1.0;
    rotation = 0;
    alpha = 1.0f;
    generate_mipmaps = False;

    set_text(_text);

}


Text::~Text()
{
    if(image != NULL)
        delete image;
    image = NULL;
}


void Text::Set_z(int new_z)
{
    z = new_z;
    Process::z = new_z;
    Process::z_order_dirty = True;
}


void Text::Set_colour(boost::python::object list)
{
    colour[0] = boost::python::extract<float>(list[0]);
    colour[1] = boost::python::extract<float>(list[1]);
    colour[2] = boost::python::extract<float>(list[2]);
}


void Text::set_text(string new_text)
{

    text = new_text;
    generate_new_text_image();

}


void Text::Set_shadow_colour(boost::python::object list)
{
    shadow_colour[0] = boost::python::extract<float>(list[0]);
    shadow_colour[1] = boost::python::extract<float>(list[1]);
    shadow_colour[2] = boost::python::extract<float>(list[2]);
}


void Text::Set_generate_mipmaps(bool generate_mipmaps_)
{
    generate_mipmaps = generate_mipmaps_;
    generate_new_text_image();
}


void Text::generate_new_text_image()
{

    if(image != NULL)
        delete image;

    if(font == NULL || text == "")
    {
        image = NULL;
        return;
    }

    // Create a new SDL texture to put our image in.
    SDL_Color colour = {255, 255, 255};
    SDL_Surface *text_surface = TTF_RenderText_Blended(font->font, text.c_str(), colour);

    // We need to work out the nearest power of 2 so
    // the texture we generate is valid
    int width = text_surface->w;
    int height = text_surface->h;

    text_width = width;
    text_height = height;
                       
    int h = 16;
    while(h < height)
        h = h * 2;
    int w = 16;
    while(w < width)
        w = w * 2;

    SDL_Surface *final_surface;

    final_surface = text_surface;
    final_surface->refcount++;

    image = new Image(final_surface, generate_mipmaps);

    SDL_FreeSurface(text_surface);
    SDL_FreeSurface(final_surface);

}



tuple<float, float> Text::get_screen_draw_position()
{

    switch(alignment)
    {
    case TEXT_ALIGN_TOP:
        return tuple<float, float>(x - (text_width / 2), y);
    case TEXT_ALIGN_TOP_RIGHT:
        return tuple<float, float>(x - text_width, y);
    case TEXT_ALIGN_CENTER_LEFT:
        return tuple<float, float>(x, y - (text_height / 2));
    case TEXT_ALIGN_CENTER:
        return tuple<float, float>(x - (text_width / 2), y - (text_height / 2));
    case TEXT_ALIGN_CENTER_RIGHT:
        return tuple<float, float>(x - text_width, y - (text_height / 2));
    case TEXT_ALIGN_BOTTOM_LEFT:
        return tuple<float, float>(x, y - text_height);
    case TEXT_ALIGN_BOTTOM:
        return tuple<float, float>(x - (text_width / 2), y - text_height);
    case TEXT_ALIGN_BOTTOM_RIGHT:
        return tuple<float, float>(x - text_width, y - text_height);
    default:
        return tuple<float, float>(x, y);
    }

}


void Text::Draw()
{

    if(is_dead == True || image == NULL)
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
                               
    // Binding texture
    if(Process::current_bound_texture != image->texture)
    {
        glBindTexture(GL_TEXTURE_2D, image->texture);
        glVertexPointer(3, GL_FLOAT, 0, image->vertex_list);
        Process::current_bound_texture = image->texture;
    }

    // Shadow stuff
    if(shadow > 0)
    {
        glTranslatef(shadow, shadow, 0.0f);
        glColor4f(shadow_colour[0], shadow_colour[1], shadow_colour[2], alpha);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glTranslatef(-shadow, -shadow, 0.0f);
    }

    // Changing colour and transparency
    glColor4f(colour[0], colour[1], colour[2], alpha);

    // draw the triangle strip
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glPopMatrix();

}


/*
 *
 */
void TextWrapper::Execute(){}
void TextWrapper::Execute_default()
{
    this->Text::Execute();
}



void TextWrapper::Kill()
{
    is_dead = True;
    Process::internal_list.remove(self_);
    this->Process::Kill();
    boost::python::decref(self);
    boost::python::decref(self);
    self = NULL;
}


TextWrapper::TextWrapper(PyObject* _self, Font* _font, float _x, float _y, int _alignment, string _text) : Text(_font, _x, _y, _alignment, _text)
{
    self = _self;
    self_ = boost::python::object(boost::python::handle<>(boost::python::borrowed(self)));
    Process::internal_list.append(self_);
}

