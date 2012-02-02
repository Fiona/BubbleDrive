/****************************
 BUBBLE DRIVE
 2011/2012 STOMPY BLONDIE GAMES
 ****************************
 Process draw strats source file
 ****************************/


#include "main.h"


/*
 * Used by the buttons in the gui system
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


/*
 * Used by window frames in the gui system
 */
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


/*
 * Used to just draw a single square
 */
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


/*
 * used to draw the nebula/star space background
 */
void Process::Draw_strategy_background()
{

    Main_App* core = Main_App::Instance();
    string nebula_image = boost::python::extract<string>(self_.attr("draw_strategy_nebula_image"));

    // If we need to regenerate verticies
    if(vertex_list_1.size() == 0)
    {

        float draw_x;
        float draw_y;
        float texture_x_from;
        float texture_x_to;

        int screen_width = core->settings->screen_width;
        int screen_height = core->settings->screen_height;

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
                        (512.0 * layer) / core->media->gfx["background_nebula" + nebula_image]->surface_width
                        );
                    texture_x_from = (
                        (512.0 * (layer + 1)) / core->media->gfx["background_nebula" + nebula_image]->surface_width
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

        float tex_coords_x = (float)screen_width / (float)core->media->gfx["background_stars"]->width;
        float tex_coords_y = (float)screen_height / (float)core->media->gfx["background_stars"]->height;

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
    glBindTexture(GL_TEXTURE_2D, core->media->gfx["background_nebula" + nebula_image]->texture);
    glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    glTexCoordPointer(2, GL_FLOAT, 0, &texture_coord_list_1[0]);
    glVertexPointer(2, GL_FLOAT, 0, &vertex_list_1[0]);
    glDrawArrays(GL_QUADS, 0, vertex_list_1.size() / 2);

    // Draw the stars
    glBindTexture(GL_TEXTURE_2D, core->media->gfx["background_stars"]->texture);
    glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
    glTexCoordPointer(2, GL_FLOAT, 0, &texture_coord_list_2[0]);
    glVertexPointer(2, GL_FLOAT, 0, &vertex_list_2[0]);
    glDrawArrays(GL_QUADS, 0, vertex_list_2.size() / 2);

    // Reset matrix
    glPopMatrix();
    Process::current_bound_texture = -1;
    glTexCoordPointer(2, GL_FLOAT, 0, &Process::default_texture_coords[0]);

}
