/****************************
 BUBBLE DRIVE
 2011/2012 STOMPY BLONDIE GAMES
 ****************************
 Processesss
 ****************************/


#ifndef _PROCESS_H_
#define _PROCESS_H_

using namespace boost;

#include <vector>
#include <SDL/SDL.h>


#define TEXT_ALIGN_TOP_LEFT 0
#define TEXT_ALIGN_TOP 1
#define TEXT_ALIGN_TOP_RIGHT 2
#define TEXT_ALIGN_CENTER_LEFT 3
#define TEXT_ALIGN_CENTER 4
#define TEXT_ALIGN_CENTER_RIGHT 5
#define TEXT_ALIGN_BOTTOM_LEFT 6
#define TEXT_ALIGN_BOTTOM 7
#define TEXT_ALIGN_BOTTOM_RIGHT 8 

#define Z_TEXT -512

struct ProcessWrapper;

/*
 */
class Process
{

public:
    Process();
    virtual ~Process();

    static boost::python::list internal_list;

    static std::vector<Process*> Process_List;
    static bool z_order_dirty;
    static std::vector<Process*> Priority_List;
    static bool priority_order_dirty;
    static GLuint current_bound_texture;

    static vector<float> default_texture_coords;
    static std::vector<Process*> Processes_to_kill;

    float   x;
    float   y;
    int     z;
    int priority;
    Image*  image;
    float   scale;
    int rotation;
    std::vector<float> colour;
    float alpha;
    int image_sequence;
    std::vector<float> scale_pos;
    
    bool is_dead;

    string draw_strategy;
    PyObject* self;
    boost::python::object self_;

    virtual void Execute();
    virtual void On_Exit();
    virtual void Draw();

    void Kill();

    void Set_z(int new_z);
    void Set_priority(int priority_);
    void Set_colour(boost::python::object list);
    void Set_scale_pos(boost::python::object list);

    void move_forward(float distance_to_travel, int rotation_to_move_in);

    virtual tuple<float, float> get_screen_draw_position();

    // Draw strategies
    vector<float>vertex_list_1;
    vector<float>vertex_list_2;
    vector<float>texture_coord_list_1;
    vector<float>texture_coord_list_2;

    void Draw_strategy_primitive_square();
    void Draw_strategy_gui_button();
    void Draw_strategy_gui_window_frame();
    void Draw_strategy_background();

};


struct ProcessWrapper : Process
{

    ProcessWrapper(PyObject *p);
    ProcessWrapper();

    bool has_init;
    bool has_killed;
    PyObject *self;
    boost::python::object self_;

    void Execute();
    void Execute_default();
    void On_Exit();
    void On_Exit_default();
    void Kill();
    tuple<float, float> get_screen_draw_position();
    tuple<float, float> get_screen_draw_position_default();

};


/*
 */
class Text: public Process
{

public:
    Text();
    Text(Font* _font, float _x, float _y, int _alignment, string _text);
    ~Text();

    float   x;
    float   y;
    int     z;
    int     priority;
    float   scale;
    int rotation;
    float alpha;

    Font* font;
    int alignment;    
    string text;
    int text_width;
    int text_height;
    int shadow;
    std::vector<float> shadow_colour;
    bool generate_mipmaps;

    void Set_z(int new_z);
    void set_text(string _text);
    void Set_colour(boost::python::object list);
    void Set_shadow_colour(boost::python::object list);
    void Set_generate_mipmaps(bool generate_mipmaps_);
    tuple<float, float> get_screen_draw_position();

    void Draw();
    void Kill();

private:
    void generate_new_text_image();

};


struct TextWrapper : Text
{

    TextWrapper(PyObject *p);
    TextWrapper(PyObject *p, Font* _font, float _x, float _y, int _alignment, string _text);

    PyObject *self;
    boost::python::object self_;

    void Execute();
    void Execute_default();
    void Kill();

};



/*
 * World objects are visual objects that will also take
 * care of position in the world relative to the camera.
 * Call init(); at the start of the process creation (after
 * overriding the world object default settings)
 */
class World_object : virtual public Process
{
public:
    World_object();

    /* Instead of setting .scale, it is necessary to set custom_scale
       this is because Myrmidon uses .scale to determine the draw
       scale of a process, but we want to set the scale in relation to
       the world. This will allow us to do that. */
    float custom_scale;

    /* Used in rectangle collisions and working out world grid coordinates. */
    bool recalculate_corner_vectors;
    vector< vector<float> > corner_vectors;

    /*Set to true to allow this object to colide with others in the world */
    bool collidable;

    /* Set the type of collision model this process will use */
    int collision_type;

    /* Used as an optimisation in rectangle collisions. Equals the distance between the
       centre of the object and a corner when set. */
    float collision_rectangle_radius;

    /* Used as an optimisation in point collisions. Is the rotated point of colission. */
    tuple<float, float> world_collision_point;
    
    /* Used for point collisions. The offset of the collision point as a tuple. 
       0 will use image centre. */
    tuple<float, float> collision_point;
    
    /* Used for rectangle collisions. Width/height of the object in world coordinates. 
       -1 will use the image size. */
    int width;
    int height;

    /* Used for circle collisions. Radius of the bounding circle in world coordinates. 
       -1 will use the image size. */
    int radius;

    /* Player can target this object if true */
    bool targetable;

    /* Set to a prefix string for the target image. 
       If "" it assumes the object is a ship which are all prefixed with "ship_" */
    string target_image_prefix;

    /* Set to true if we want this to appear on the minimap */
    bool show_on_minimap;

    /* We need to differenciate between ships and other world objects */
    bool is_ship;
    
    /* Used to identify an object by name when targeting */
    string object_name;

    /* Faction is used for targeting and collisions to make sure the right thing
       gets hurt. Must be set before the __init__ method. */
    int faction;

    /* structural damage of object */
    int max_health;
    int health;

    /* shields of object. -1 is no shields. */
    int max_shields;
    int shields;

    void init();
    void set_rotation(int rotation_);
    void set_x(float x_);
    void set_y(float y_);
    void Execute();
    void Destroy();
    void Kill();
    tuple<float, float> get_screen_draw_position();
    void calculate_corner_vectors();
};


struct World_objectWrapper : virtual public Process, public World_object
{
    World_objectWrapper();
    World_objectWrapper(PyObject *p);

    void Destroy_default();
    void Destroy();
    void Execute_default();
    void Execute();
    tuple<float, float> get_screen_draw_position_default();
    tuple<float, float> get_screen_draw_position();
    void Kill();
};



/*
 * Physical objects have an acceleraction and will take
 * care of their velocity, rotation and position calculations.
 * Call init(); at the start of the process creation (after overriding
 * world object and physical object default settings).
 */
class Physical_object : virtual public World_object
{
    
public:
    Physical_object();

    Vector2D* pos;
    Vector2D* velocity;

    float max_velocity;
    float velocity_friction;
    float accel;
    float rotation_velocity;
    float rotation_friction;
    float rotation_accel;

    float get_x();
    void set_x(float x_);

    float get_y();
    void set_y(float y_);

    void init();
    void update_rotation();
    void update_velocity();
    void update_position();
    void bump(Vector2D* vec);
    void Execute();

};


struct Physical_objectWrapper : virtual public Process, virtual public World_object, public Physical_object
{

public:
    Physical_objectWrapper();
    Physical_objectWrapper(PyObject *p);

    void Execute_default();
    void Execute();

};


#endif 
