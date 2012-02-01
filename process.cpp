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
std::vector<Process*> Process::Processes_to_kill;
boost::python::list Process::internal_list;

vector<float> Process::default_texture_coords(8);

bool Process::z_order_dirty;
GLuint Process::current_bound_texture = -1;


 
Process::Process()
{
    x = y = 0.0f;
    z = 0;
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


float Process::deg_to_rad(float deg)
{
    return (3.1415926f / 180.0f) * deg;
}
 

float Process::rad_to_deg(float rad)
{
    return rad * 180.0f / 3.1415926f;
}


tuple<float, float> Process::get_screen_draw_position()
{

    if(image == NULL)
        return tuple<float, float>(x, y);

    return tuple<float, float>(x - (image -> width / 2), y - (image -> height / 2));

}


/*
 * DRAW STRATEGIES
 */
void Process::Draw_strategy_gui_button()
{

    if(alpha <= 0.0)
        return;

    float width = boost::python::extract<float>(self_.attr("width"));
    float height = boost::python::extract<float>(self_.attr("height"));

    // Draw the background gradient
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);

    glTexCoordPointer(2, GL_FLOAT, 0, &image->texture_coords[image_sequence-1][0]);
                               
    glBindTexture(GL_TEXTURE_2D, image->texture);
    Process::current_bound_texture = image->texture;

    float vertex_list[12];
    for(int i = 0; i < 12; i++)
        vertex_list[i] = 0.0f;
    vertex_list[0] = (float)width;
    vertex_list[1] = (float)height;
    vertex_list[4] = (float)height;
    vertex_list[6] = (float)width; 

    glVertexPointer(3, GL_FLOAT, 0, vertex_list);

    glTranslatef(x, y, 0.0);
    glColor4f(1.0, 1.0, 1.0, alpha);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glTexCoordPointer(2, GL_FLOAT, 0, &Process::default_texture_coords[0]);

    // Draw the surrounding rectangle    
    glDisable(GL_TEXTURE_2D);
    glLineWidth(2.0);
    glColor4f(.5, .5, .5, alpha);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.0, 0.0);
    glVertex2f(width, 0.0);
    glVertex2f(width, height);
    glVertex2f(0.0, height);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    glPopMatrix();

}


void Process::Draw_strategy_gui_window_frame()
{

    if(alpha <= 0.0)
        return;

    float width = boost::python::extract<float>(self_.attr("width"));
    float height = boost::python::extract<float>(self_.attr("height"));
    tuple<float, float> draw_pos = get_screen_draw_position();
    float draw_x = draw_pos.get<0>();
    float draw_y = draw_pos.get<1>();

    glPushMatrix();

    glDisable(GL_TEXTURE_2D);

    //glVertex2f(top_left[0], top_left[1])
    //glVertex2f(bottom_right[0], top_left[1])
    //glVertex2f(bottom_right[0], bottom_right[1])
    //glVertex2f(top_left[0], bottom_right[1])

    // Background grey shadow
    glColor4f(0.7, 0.7, 0.7, 0.7);
    glBegin(GL_QUADS);
    glVertex2f(draw_x + 12.0f,        draw_y + 12.0f);
    glVertex2f(draw_x + width - 4.0f, draw_y + 12.0f);
    glVertex2f(draw_x + width - 4.0f, draw_y + height - 4.0f);
    glVertex2f(draw_x + 12.0f,        draw_y + height - 4.0f);
    glEnd();

    // Background of frame
    glBegin(GL_QUADS);
    glColor4f(0.8, 1.0, 1.0, 1.0);
    glVertex2f(draw_x + 8.0f,          draw_y + 8.0f);
    glVertex2f(draw_x + width - 8.0f,  draw_y + 8.0f);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glVertex2f(draw_x + width - 8.0f,  draw_y + height - 8.0f);
    glVertex2f(draw_x + 8.0f,          draw_y + height - 8.0f);
    glEnd();

    // Frame border
    glColor4f(0.7, 0.7, 0.7, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(draw_x + 10.0f,          draw_y + 10.0f);
    glVertex2f(draw_x + width - 10.0f,  draw_y + 10.0f);
    glVertex2f(draw_x + width - 10.0f,  draw_y + height - 10.0f);
    glVertex2f(draw_x + 10.0f,          draw_y + height - 10.0f);
    glEnd();

    // Title shadow
    glColor4f(0.7, 0.7, 0.7, 0.7);
    glBegin(GL_QUADS);
    glVertex2f(draw_x + 16.0f,   draw_y + 6.0f);
    glVertex2f(draw_x + 250.0f,  draw_y + 6.0f);
    glVertex2f(draw_x + 250.0f,  draw_y + 20.0f);
    glVertex2f(draw_x + 16.0f,   draw_y + 20.0f);
    glEnd();

    // Title 
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(draw_x + 10.0f,   draw_y);
    glVertex2f(draw_x + 244.0f,  draw_y);
    glVertex2f(draw_x + 244.0f,  draw_y + 16.0f);
    glVertex2f(draw_x + 10.0f,   draw_y + 16.0f);
    glEnd();

    glEnable(GL_TEXTURE_2D);

    glPopMatrix();

}


void Process::Draw_strategy_primitive_square()
{

    float square_x = boost::python::extract<float>(self_.attr("primitive_square_x"));
    float square_y = boost::python::extract<float>(self_.attr("primitive_square_y"));
    float width = boost::python::extract<float>(self_.attr("primitive_square_width"));
    float height = boost::python::extract<float>(self_.attr("primitive_square_height"));
    boost::python::tuple colour = boost::python::extract<boost::python::tuple>(self_.attr("primitive_square_colour"))();

    bool filled;
    if(hasattr(self_, "primitive_square_filled"))
        filled = boost::python::extract<bool>(self_.attr("primitive_square_filled"));
    else
        filled = True;

    bool four_colours;
    if(hasattr(self_, "primitive_square_four_colours"))
        four_colours = boost::python::extract<bool>(self_.attr("primitive_square_four_colours"));
    else
        four_colours = False;

    glPushMatrix();

    glDisable(GL_TEXTURE_2D);

    if(filled)
        glBegin(GL_QUADS);
    else
    {

        float line_width;
        if(hasattr(self_, "primitive_square_line_width"))
            line_width = boost::python::extract<float>(self_.attr("primitive_square_line_width"));
        else
            line_width = 1.0;

        glLineWidth(line_width);
        glBegin(GL_LINE_LOOP);

    }

    boost::python::tuple col_1;
    if(four_colours)
        col_1 = boost::python::extract<boost::python::tuple>(colour[0]);
    else
        col_1 = colour;
    glColor4f(
        boost::python::extract<float>(col_1[0]),
        boost::python::extract<float>(col_1[1]),
        boost::python::extract<float>(col_1[2]),
        boost::python::extract<float>(col_1[3])
        );

    glVertex2f(square_x, square_y);

    if(four_colours)
    {
        col_1 = boost::python::extract<boost::python::tuple>(colour[1]);
        glColor4f(
            boost::python::extract<float>(col_1[0]),
            boost::python::extract<float>(col_1[1]),
            boost::python::extract<float>(col_1[2]),
            boost::python::extract<float>(col_1[3])
            );
    }

    glVertex2f(square_x + width, square_y);

    if(four_colours)
    {
        col_1 = boost::python::extract<boost::python::tuple>(colour[2]);
        glColor4f(
            boost::python::extract<float>(col_1[0]),
            boost::python::extract<float>(col_1[1]),
            boost::python::extract<float>(col_1[2]),
            boost::python::extract<float>(col_1[3])
            );
    }

    glVertex2f(square_x + width, square_y + height);

    if(four_colours)
    {
        col_1 = boost::python::extract<boost::python::tuple>(colour[3]);
        glColor4f(
            boost::python::extract<float>(col_1[0]),
            boost::python::extract<float>(col_1[1]),
            boost::python::extract<float>(col_1[2]),
            boost::python::extract<float>(col_1[3])
            );
    }

    glVertex2f(square_x, square_y + height);
    glEnd();
                                          
    glEnable(GL_TEXTURE_2D);

    glPopMatrix();

    bool call_parent;
    if(hasattr(self_, "draw_strategy_call_parent"))
        call_parent = boost::python::extract<bool>(self_.attr("draw_strategy_call_parent"));
    else
        call_parent = True;

    if(call_parent)
        this->Draw();

}


void Process::Draw_strategy_background()
{

    boost::python::object game = boost::python::extract<boost::python::object>(self_.attr("game"));
    boost::python::object core = boost::python::extract<boost::python::object>(game.attr("core"));
    Media* media = boost::python::extract<Media*>(core.attr("media"));

    string nebula_image = boost::python::extract<string>(self_.attr("draw_strategy_nebula_image"));

    // If we need to regenerate verticies
    if(vertex_list_1.size() == 0)
    {

        float draw_x;
        float draw_y;
        float texture_x_from;
        float texture_x_to;

        int screen_width = boost::python::extract<float>(self_.attr("draw_strategy_screen_width"));
        int screen_height = boost::python::extract<float>(self_.attr("draw_strategy_screen_height"));

        for(int i = 0; i < 5; i++)
        {

            for(int layer = 0; layer < BACKGROUND_NUM_NEBULA_ITEMS; layer++)
            {

                draw_x = (float)randint(-256, screen_width + 256);
                draw_y = (float)randint(-256, screen_height + 256);

                for(int junk = 0; junk < 2; junk++)
                {

                    vertex_list_1.push_back(draw_x + 512.0f);
                    vertex_list_1.push_back(draw_y);

                    vertex_list_1.push_back(draw_x);
                    vertex_list_1.push_back(draw_y);

                    vertex_list_1.push_back(draw_x);
                    vertex_list_1.push_back(draw_y + 512.0f);

                    vertex_list_1.push_back(draw_x + 512.0f);
                    vertex_list_1.push_back(draw_y + 512.0f);

                    texture_x_to = (
                        (512.0 * layer) / media->gfx["background_nebula" + nebula_image]->surface_width
                        );
                    texture_x_from = (
                        (512.0 * (layer + 1)) / media->gfx["background_nebula" + nebula_image]->surface_width
                        );

                    texture_coord_list_1.push_back(texture_x_to);
                    texture_coord_list_1.push_back(0.0f);
                    
                    texture_coord_list_1.push_back(texture_x_from);
                    texture_coord_list_1.push_back(0.0f);

                    texture_coord_list_1.push_back(texture_x_from);
                    texture_coord_list_1.push_back(1.0f);
                
                    texture_coord_list_1.push_back(texture_x_to);
                    texture_coord_list_1.push_back(1.0f);

                }

            }

        }

        // star verticies and texture coords
        vertex_list_2.push_back(screen_width);
        vertex_list_2.push_back(0.0f);

        vertex_list_2.push_back(0.0f);
        vertex_list_2.push_back(0.0f);

        vertex_list_2.push_back(0.0f);
        vertex_list_2.push_back(screen_height);

        vertex_list_2.push_back(screen_width);
        vertex_list_2.push_back(screen_height);

        float tex_coords_x = (float)screen_width / (float)media->gfx["background_stars"]->width;
        float tex_coords_y = (float)screen_height / (float)media->gfx["background_stars"]->height;

        texture_coord_list_2.push_back(tex_coords_x);
        texture_coord_list_2.push_back(0.0f);

        texture_coord_list_2.push_back(0.0f);
        texture_coord_list_2.push_back(0.0f);

        texture_coord_list_2.push_back(0.0f);
        texture_coord_list_2.push_back(tex_coords_y);

        texture_coord_list_2.push_back(tex_coords_x);
        texture_coord_list_2.push_back(tex_coords_y);

    }

    // Draw the nebulas
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, media->gfx["background_nebula" + nebula_image]->texture);
    glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    glTexCoordPointer(2, GL_FLOAT, 0, &texture_coord_list_1[0]);
    glVertexPointer(2, GL_FLOAT, 0, &vertex_list_1[0]);
    glDrawArrays(GL_QUADS, 0, vertex_list_1.size() / 2);

    // Draw the stars
    glBindTexture(GL_TEXTURE_2D, media->gfx["background_stars"]->texture);
    glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
    glTexCoordPointer(2, GL_FLOAT, 0, &texture_coord_list_2[0]);
    glVertexPointer(2, GL_FLOAT, 0, &vertex_list_2[0]);
    glDrawArrays(GL_QUADS, 0, vertex_list_2.size() / 2);

    // Reset matrix
    glPopMatrix();
    Process::current_bound_texture = -1;
    glTexCoordPointer(2, GL_FLOAT, 0, &Process::default_texture_coords[0]);

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

