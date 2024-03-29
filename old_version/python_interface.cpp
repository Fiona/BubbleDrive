/****************************
 BUBBLE DRIVE
 2011/2012 STOMPY BLONDIE GAMES
 ****************************
 Handles all the python interfacing code
 ****************************/

#include <iostream>
using namespace std;
#include "main.h"


using namespace boost::python;

namespace my
{

    boost::python::object tuple_to_python(boost::tuples::null_type)
    {
        return boost::python::tuple();
    }

    template <class H, class T>
    boost::python::object tuple_to_python(boost::tuples::cons<H,T> const& x)
    {
        return boost::python::make_tuple(x.get_head()) + my::tuple_to_python(x.get_tail());
    }

    template <class T>
    struct tupleconverter
    {
        static PyObject* convert(T const& x)
        {
            return incref(my::tuple_to_python(x).ptr());
        }
    };


	struct Process_to_python_object
	{
	    static PyObject* convert(Process* p)
        {
	        return boost::python::incref(p->self_.ptr());
        }
	};

}


/*
 * This is the code that creates the game_core module.
 * It exposes all the objects and methods that Python needs
 * to be able to access.
 */
BOOST_PYTHON_MODULE(core)
{

    typedef boost::tuples::tuple<float, float> screen_pos_tuple;
    to_python_converter<screen_pos_tuple, my::tupleconverter<screen_pos_tuple> >();
    typedef boost::tuples::tuple<int, int> int_int_tuple;
    to_python_converter<int_int_tuple, my::tupleconverter<int_int_tuple> >();
    
    boost::python::to_python_converter<Process*, my::Process_to_python_object>();

    // Expose the mathutil vector class
    class_<Vector2D>("Vector2D")
        .def(init<float, float, boost::python::optional<bool> >())
        .def("get_mag", &Vector2D::get_mag)
        .def("to_tuple", &Vector2D::to_tuple)
        .def("dot", &Vector2D::dot)

        .def(self + Vector2D())              // __add__
        .def(Vector2D() + self)              // __radd__
        .def(self += Vector2D())             // __iadd__
        .def(self - Vector2D())              // __sub__
        .def(Vector2D() - self)              // __rsub__
        .def(self -= Vector2D())             // __isub__
        .def(self * Vector2D())              // __mul__
        .def(Vector2D() * self)              // __rmul__
        .def(self *= Vector2D())             // __imul__
        .def(self / Vector2D())              // __div__
        .def(Vector2D() / self)              // __rdiv__
        .def(self /= Vector2D())             // __idiv__
        .def(self == Vector2D())             // __eq__
        .def(self != Vector2D())             // __neq__
        //having to explicitly specify the namespace is a boost bug
        .def(self_ns::str(self_ns::self))    // __str__  

        .add_property("x", make_getter(&Vector2D::x), make_setter(&Vector2D::x))
        .add_property("y", make_getter(&Vector2D::y), make_setter(&Vector2D::y))
        ;

    register_ptr_to_python< shared_ptr<Vector2D> >();

    // Expose all media related objects
    class_<Image>("Image")
        .def_readonly("num_of_frames", &Image::num_of_frames)
        .def_readonly("width", &Image::width)
        .def_readonly("height", &Image::height)
        ;
    class_<Font>("Font");

    STL_MAP_WRAPPING_PTR(string, Image*, "gfxMap");
    STL_MAP_WRAPPING_PTR(string, Font*, "fontMap");
    class_<Media>("Media")
        .def_readonly("gfx", &Media::gfx)
        .def_readonly("fonts", &Media::fonts)
        ;

    // Expose settings object
    class_<Settings>("Settings")
        .add_property("screen_width", make_getter(&Settings::screen_width), make_setter(&Settings::screen_width))
        .add_property("screen_height", make_getter(&Settings::screen_height), make_setter(&Settings::screen_height))
        .add_property("full_screen", make_getter(&Settings::full_screen), make_setter(&Settings::full_screen))
        .add_property("key_ship_forward", make_getter(&Settings::key_ship_forward), make_setter(&Settings::key_ship_forward))
        .add_property("key_ship_back", make_getter(&Settings::key_ship_back), make_setter(&Settings::key_ship_back))
        .add_property("key_ship_left", make_getter(&Settings::key_ship_left), make_setter(&Settings::key_ship_left))
        .add_property("key_ship_right", make_getter(&Settings::key_ship_right), make_setter(&Settings::key_ship_right))
        .add_property("key_zoom_in", make_getter(&Settings::key_zoom_in), make_setter(&Settings::key_zoom_in))
        .add_property("key_zoom_out", make_getter(&Settings::key_zoom_out), make_setter(&Settings::key_zoom_out))
        ;

    // Expose common vectors
    boost::python::class_< std::vector<float> >("FloatVector")
        .def(boost::python::vector_indexing_suite<std::vector<float> >());

    boost::python::class_< std::vector< std::vector<float> > >("FloatVectorVector")
        .def(boost::python::vector_indexing_suite<std::vector< std::vector<float> > >());

    boost::python::class_< std::vector<Process*> >("ProcessVector")
        .def(boost::python::vector_indexing_suite< std::vector<Process*> >());

    boost::python::class_< std::vector< std::vector<Process*> > >("ProcessVectorVector")
        .def(boost::python::vector_indexing_suite< std::vector< std::vector<Process*> > >());

    // Expose Process object
    class_<Process, ProcessWrapper, boost::noncopyable, boost::shared_ptr<ProcessWrapper> >("Process", init<>())
        .def("Execute", &Process::Execute, &ProcessWrapper::Execute_default)
        .def("On_Exit", &Process::On_Exit, &ProcessWrapper::On_Exit_default)
        .def("get_screen_draw_position", &Process::get_screen_draw_position, &ProcessWrapper::get_screen_draw_position_default)

        .add_property("x", make_getter(&Process::x), make_setter(&Process::x))
        .add_property("y", make_getter(&Process::y), make_setter(&Process::y))
        .add_property("z", make_getter(&Process::z), &Process::Set_z)
        .add_property("priority", make_getter(&Process::priority), &Process::Set_priority)
        .add_property("colour", make_getter(&Process::colour), &Process::Set_colour)
        .add_property("alpha", make_getter(&Process::alpha), make_setter(&Process::alpha))
        .add_property("scale", make_getter(&Process::scale), make_setter(&Process::scale))
        .add_property("rotation", make_getter(&Process::rotation), make_setter(&Process::rotation))
        .add_property("image_sequence", make_getter(&Process::image_sequence), make_setter(&Process::image_sequence))
        .add_property("scale_pos", make_getter(&Process::scale_pos), &Process::Set_scale_pos)
        .add_property("draw_strategy", make_getter(&Process::draw_strategy), make_setter(&Process::draw_strategy))

        .add_property(
            "image",
            make_getter(&Process::image, return_value_policy<reference_existing_object>()),
            make_setter(&Process::image)
            )

        .def("move_forward", &Process::move_forward)
        .def("get_distance", &Process::get_distance)
        .def("Kill", &ProcessWrapper::Kill)
        ;

    // Expose Text object
    class_<Text, TextWrapper, boost::noncopyable, boost::shared_ptr<TextWrapper> >("Text", init< Font*, float, float, int, string >())
        .add_property(
            "text",
            make_getter(&Text::text),
            &Text::set_text
            )
        .add_property("x", make_getter(&Text::x), make_setter(&Text::x))
        .add_property("y", make_getter(&Text::y), make_setter(&Text::y))
        .add_property("z", make_getter(&Text::z), &Text::Set_z)
        .add_property("alpha", make_getter(&Text::alpha), make_setter(&Text::alpha))
        .add_property("scale", make_getter(&Text::scale), make_setter(&Text::scale))
        .add_property("rotation", make_getter(&Text::rotation), make_setter(&Text::rotation))
        .add_property("colour", make_getter(&Text::colour), &Text::Set_colour)
        .add_property("shadow", make_getter(&Text::shadow), make_setter(&Text::shadow))
        .add_property("shadow_colour", make_getter(&Text::shadow_colour), &Text::Set_shadow_colour)
        .add_property("text_width", make_getter(&Text::text_width), make_setter(&Text::text_width))
        .add_property("text_height", make_getter(&Text::text_height), make_setter(&Text::text_height))
        .add_property("generate_mipmaps", make_getter(&Text::generate_mipmaps), &Text::Set_generate_mipmaps)
        .def("Kill", &TextWrapper::Kill)
        ;

    // Expose the mouse class
    class_<Mouse>("Mouse")
        .add_property("x", make_getter(&Mouse::x))
        .add_property("y", make_getter(&Mouse::y))
        .add_property("x_rel", make_getter(&Mouse::x_rel))
        .add_property("y_rel", make_getter(&Mouse::y_rel))
        .add_property("left_down", make_getter(&Mouse::left_down))
        .add_property("left_up", make_getter(&Mouse::left_up))
        .add_property("right_down", make_getter(&Mouse::right_down))
        .add_property("right_up", make_getter(&Mouse::right_up))
        .add_property("middle_down", make_getter(&Mouse::middle_down))
        .add_property("middle_up", make_getter(&Mouse::middle_up))
        .add_property("wheel_down", make_getter(&Mouse::wheel_down))
        .add_property("wheel_up", make_getter(&Mouse::wheel_up))
        .def("set_pos", &Mouse::set_pos)
        ;

    // Expose the main app obj
    class_<Main_App>("Main_App")
        .add_property("mouse", make_getter(&Main_App::mouse, return_value_policy<reference_existing_object>()))
        .add_property("current_fps", make_getter(&Main_App::current_fps))
        .add_property("process_count", make_getter(&Main_App::process_count))
        .add_property("media", make_getter(&Main_App::media, return_value_policy<reference_existing_object>()))
        .add_property("settings", make_getter(&Main_App::settings, return_value_policy<reference_existing_object>()))
        .add_property("path_application_data", make_getter(&Main_App::path_application_data))
        .add_property("path_settings_file", make_getter(&Main_App::path_settings_file))


        .def( "Keyboard_key_down", static_cast< bool (Main_App::*)(int) >(&Main_App::Keyboard_key_down))
        .def( "Keyboard_key_released", static_cast< bool (Main_App::*)(SDLKey) >(&Main_App::Keyboard_key_released)) 

        //#.def("Keyboard_key_down", &Main_App::Keyboard_key_down)
        //.def("Keyboard_key_released", &Main_App::Keyboard_key_released)
        .def("Quit", &Main_App::Quit)

        .add_property("camera_x", make_getter(&Main_App::camera_x), make_setter(&Main_App::camera_x))
        .add_property("camera_y", make_getter(&Main_App::camera_y), make_setter(&Main_App::camera_y))
        .add_property("global_scale", make_getter(&Main_App::global_scale), make_setter(&Main_App::global_scale))
        .add_property("world_objects", make_getter(&Main_App::world_objects))
        .add_property("targetable_world_objects", make_getter(&Main_App::targetable_world_objects))
        .add_property("world_objects_by_faction", make_getter(&Main_App::world_objects_by_faction))

        .def("get_distance", &Main_App::get_distance)
        .def("angle_between_points", &Main_App::angle_between_points)
        .def("normalise_angle", &Main_App::normalise_angle)
        .def("angle_difference", &Main_App::angle_difference)
        .def("near_angle", &Main_App::near_angle)
        .def("rotate_point", &Main_App::rotate_point)
        .def("rotate_point_about_point", &Main_App::rotate_point_about_point)

        .def("screen_to_world", &Main_App::screen_to_world)
        .def("world_to_screen", &Main_App::world_to_screen)
        .def("world_to_in_universe_coords", &Main_App::world_to_in_universe_coords)
        ;

    // Bubble Drive specific processes
    class_<World_object, boost::python::bases<Process>, World_objectWrapper, boost::noncopyable>("World_object", init<>())
        .add_property("x", make_getter(&World_object::x), &World_object::set_x)
        .add_property("y", make_getter(&World_object::y), &World_object::set_y)
        .add_property("rotation", make_getter(&World_object::rotation), &World_object::set_rotation)

        .add_property("recalculate_corner_vectors", make_getter(&World_object::recalculate_corner_vectors), make_setter(&World_object::recalculate_corner_vectors))
        .add_property("corner_vectors", make_getter(&World_object::corner_vectors), make_setter(&World_object::corner_vectors))
        .add_property("collidable", make_getter(&World_object::collidable), make_setter(&World_object::collidable))
        .add_property("collision_type", make_getter(&World_object::collision_type), make_setter(&World_object::collision_type))
        .add_property("collision_rectangle_radius", make_getter(&World_object::collision_rectangle_radius), make_setter(&World_object::collision_rectangle_radius))
        .add_property("world_collision_point", make_getter(&World_object::world_collision_point), make_setter(&World_object::world_collision_point))
        .add_property("collision_point", make_getter(&World_object::collision_point), make_setter(&World_object::collision_point))
        .add_property("width", make_getter(&World_object::width), make_setter(&World_object::width))
        .add_property("height", make_getter(&World_object::height), make_setter(&World_object::height))
        .add_property("radius", make_getter(&World_object::radius), make_setter(&World_object::radius))
        .add_property("targetable", make_getter(&World_object::targetable), make_setter(&World_object::targetable))
        .add_property("target_image_prefix", make_getter(&World_object::target_image_prefix), make_setter(&World_object::target_image_prefix))
        .add_property("show_on_minimap", make_getter(&World_object::show_on_minimap), make_setter(&World_object::show_on_minimap))
        .add_property("is_ship", make_getter(&World_object::is_ship), make_setter(&World_object::is_ship))
        .add_property("object_name", make_getter(&World_object::object_name), make_setter(&World_object::object_name))
        .add_property("faction", make_getter(&World_object::faction), make_setter(&World_object::faction))
        .add_property("max_health", make_getter(&World_object::max_health), make_setter(&World_object::max_health))
        .add_property("health", make_getter(&World_object::health), make_setter(&World_object::health))
        .add_property("max_shields", make_getter(&World_object::max_shields), make_setter(&World_object::max_shields))
        .add_property("shields", make_getter(&World_object::shields), make_setter(&World_object::shields))

        .def("init", &World_object::init)
        .def("Execute", &World_object::Execute, &World_objectWrapper::Execute_default)
        .def("Destroy", &World_object::Destroy, &World_objectWrapper::Destroy_default)
        .def("get_screen_draw_position", &World_object::get_screen_draw_position, &World_objectWrapper::get_screen_draw_position_default)
        .def("Kill", &World_objectWrapper::Kill)
        ;

    class_<Physical_object, boost::python::bases<World_object>, Physical_objectWrapper, boost::noncopyable>("Physical_object", init<>())
        .add_property("x", make_getter(&Physical_object::x), &Physical_object::set_x)
        .add_property("y", make_getter(&Physical_object::y), &Physical_object::set_y)

        .add_property("pos", make_getter(&Physical_object::pos, return_value_policy<reference_existing_object>()), make_setter(&Physical_object::pos))
        .add_property("velocity", make_getter(&Physical_object::velocity, return_value_policy<reference_existing_object>()), make_setter(&Physical_object::velocity))
        .add_property("max_velocity", make_getter(&Physical_object::max_velocity), make_setter(&Physical_object::max_velocity))
        .add_property("velocity_friction", make_getter(&Physical_object::velocity_friction), make_setter(&Physical_object::velocity_friction))
        .add_property("accel", make_getter(&Physical_object::accel), make_setter(&Physical_object::accel))
        .add_property("rotation_velocity", make_getter(&Physical_object::rotation_velocity), make_setter(&Physical_object::rotation_velocity))
        .add_property("rotation_friction", make_getter(&Physical_object::rotation_friction), make_setter(&Physical_object::rotation_friction))
        .add_property("rotation_accel", make_getter(&Physical_object::rotation_accel), make_setter(&Physical_object::rotation_accel))

        .def("init", &Physical_object::init)
        .def("update_position", &Physical_object::update_position)
        .def("update_velocity", &Physical_object::update_velocity)
        .def("update_position", &Physical_object::update_position)
        .def("bump", &Physical_object::bump)

        .def("Execute", &Physical_object::Execute, &Physical_objectWrapper::Execute_default)
        ;

    // Expose the framework constants
    scope().attr("TEXT_ALIGN_TOP_LEFT") = TEXT_ALIGN_TOP_LEFT;
    scope().attr("TEXT_ALIGN_TOP") = TEXT_ALIGN_TOP;
    scope().attr("TEXT_ALIGN_TOP_RIGHT") = TEXT_ALIGN_TOP_RIGHT;
    scope().attr("TEXT_ALIGN_CENTER_LEFT") = TEXT_ALIGN_CENTER_LEFT;
    scope().attr("TEXT_ALIGN_CENTER") = TEXT_ALIGN_CENTER;
    scope().attr("TEXT_ALIGN_CENTER_RIGHT") = TEXT_ALIGN_CENTER_RIGHT;
    scope().attr("TEXT_ALIGN_BOTTOM_LEFT") = TEXT_ALIGN_BOTTOM_LEFT;
    scope().attr("TEXT_ALIGN_BOTTOM") = TEXT_ALIGN_BOTTOM;
    scope().attr("TEXT_ALIGN_BOTTOM_RIGHT") = TEXT_ALIGN_BOTTOM_RIGHT;

    // Expose some Bubble Drive constants
    scope().attr("BACKGROUND_NUM_NEBULA_TYPES") = BACKGROUND_NUM_NEBULA_TYPES;
    scope().attr("COLLISION_TYPE_RECTANGLE") = COLLISION_TYPE_RECTANGLE;
    scope().attr("COLLISION_TYPE_CIRCLE") = COLLISION_TYPE_CIRCLE;
    scope().attr("COLLISION_TYPE_POINT") = COLLISION_TYPE_POINT;
    scope().attr("FACTION_NEUTRAL") = FACTION_NEUTRAL;
    scope().attr("FACTION_PLAYER") = FACTION_PLAYER;
    scope().attr("FACTION_OTHER") = FACTION_OTHER;
    scope().attr("FACTION_ENEMY") = FACTION_ENEMY;
    scope().attr("MINIMAP_RANGE") = MINIMAP_RANGE;
    scope().attr("SPACE_DUST_TYPES") = SPACE_DUST_TYPES;
    scope().attr("PRIORITY_WORLD_OBJECTS") = PRIORITY_WORLD_OBJECTS;

    // Expose all the SDL Keybinding constants
    enum_<SDLKey>("key")
        // Ascii keys
        .value("UNKNOWN", SDLK_UNKNOWN)
        .value("FIRST", SDLK_FIRST)
        .value("BACKSPACE", SDLK_BACKSPACE)
        .value("TAB", SDLK_TAB)
        .value("CLEAR", SDLK_CLEAR)
        .value("RETURN", SDLK_RETURN)
        .value("PAUSE", SDLK_PAUSE)
        .value("ESCAPE",SDLK_ESCAPE) 
        .value("SPACE", SDLK_SPACE)
        .value("EXCLAIM", SDLK_EXCLAIM)
        .value("QUOTEDBL", SDLK_QUOTEDBL)
        .value("HASH", SDLK_HASH)
        .value("DOLLAR", SDLK_DOLLAR)
        .value("AMPERSAND", SDLK_AMPERSAND)
        .value("QUOTE", SDLK_QUOTE)
        .value("LEFTPAREN", SDLK_LEFTPAREN)
        .value("RIGHTPAREN", SDLK_RIGHTPAREN)
        .value("ASTERISK", SDLK_ASTERISK)
        .value("PLUS", SDLK_PLUS)
        .value("COMMA", SDLK_COMMA)
        .value("MINUS", SDLK_MINUS)
        .value("PERIOD", SDLK_PERIOD)
        .value("SLASH", SDLK_SLASH)
        .value("0", SDLK_0)
        .value("1", SDLK_1)
        .value("2", SDLK_2)
        .value("3", SDLK_3)
        .value("4", SDLK_4)
        .value("5", SDLK_5)
        .value("6", SDLK_6)
        .value("7", SDLK_7)
        .value("8", SDLK_8)
        .value("9", SDLK_9)
        .value("COLON", SDLK_COLON)
        .value("SEMICOLON", SDLK_SEMICOLON)
        .value("LESS", SDLK_LESS)
        .value("EQUALS", SDLK_EQUALS)
        .value("GREATER", SDLK_GREATER)
        .value("QUESTION", SDLK_QUESTION)
        .value("AT", SDLK_AT)

        // (Skip uppercase letters)
        .value("LEFTBRACKET", SDLK_LEFTBRACKET)
        .value("BACKSLASH", SDLK_BACKSLASH)
        .value("RIGHTBRACKET", SDLK_RIGHTBRACKET)
        .value("CARET", SDLK_CARET)
        .value("UNDERSCORE", SDLK_UNDERSCORE)
        .value("BACKQUOTE", SDLK_BACKQUOTE)
        .value("a", SDLK_a)
        .value("b", SDLK_b)
        .value("c", SDLK_c)
        .value("d", SDLK_d)
        .value("e", SDLK_e)
        .value("f", SDLK_f)
        .value("g", SDLK_g)
        .value("h", SDLK_h)
        .value("i", SDLK_i)
        .value("j", SDLK_j)
        .value("k", SDLK_k)
        .value("l", SDLK_l)
        .value("m", SDLK_m)
        .value("n", SDLK_n)
        .value("o", SDLK_o)
        .value("p", SDLK_p)
        .value("q", SDLK_q)
        .value("r", SDLK_r)
        .value("s", SDLK_s)
        .value("t", SDLK_t)
        .value("u", SDLK_u)
        .value("v", SDLK_v)
        .value("w", SDLK_w)
        .value("x", SDLK_x)
        .value("y", SDLK_y)
        .value("z", SDLK_z)
        .value("DELETE", SDLK_DELETE)
        // End of ASCII keys

        // International keyboard syms
        .value("WORLD_0", SDLK_WORLD_0)
        .value("WORLD_1", SDLK_WORLD_1)
        .value("WORLD_2", SDLK_WORLD_2)
        .value("WORLD_3", SDLK_WORLD_3)
        .value("WORLD_4", SDLK_WORLD_4)
        .value("WORLD_5", SDLK_WORLD_5)
        .value("WORLD_6", SDLK_WORLD_6)
        .value("WORLD_7", SDLK_WORLD_7)
        .value("WORLD_8", SDLK_WORLD_8)
        .value("WORLD_9", SDLK_WORLD_9)
        .value("WORLD_10", SDLK_WORLD_10)
        .value("WORLD_11", SDLK_WORLD_11)
        .value("WORLD_12", SDLK_WORLD_12)
        .value("WORLD_13", SDLK_WORLD_13)
        .value("WORLD_14", SDLK_WORLD_14)
        .value("WORLD_15", SDLK_WORLD_15)
        .value("WORLD_16", SDLK_WORLD_16)
        .value("WORLD_17", SDLK_WORLD_17)
        .value("WORLD_18", SDLK_WORLD_18)
        .value("WORLD_19", SDLK_WORLD_19)
        .value("WORLD_20", SDLK_WORLD_20)
        .value("WORLD_21", SDLK_WORLD_21)
        .value("WORLD_22", SDLK_WORLD_22)
        .value("WORLD_23", SDLK_WORLD_23)
        .value("WORLD_24", SDLK_WORLD_24)
        .value("WORLD_25", SDLK_WORLD_25)
        .value("WORLD_26", SDLK_WORLD_26)
        .value("WORLD_27", SDLK_WORLD_27)
        .value("WORLD_28", SDLK_WORLD_28)
        .value("WORLD_29", SDLK_WORLD_29)
        .value("WORLD_30", SDLK_WORLD_30)
        .value("WORLD_31", SDLK_WORLD_31)
        .value("WORLD_32", SDLK_WORLD_32)
        .value("WORLD_33", SDLK_WORLD_33)
        .value("WORLD_34", SDLK_WORLD_34)
        .value("WORLD_35", SDLK_WORLD_35)
        .value("WORLD_36", SDLK_WORLD_36)
        .value("WORLD_37", SDLK_WORLD_37)
        .value("WORLD_38", SDLK_WORLD_38)
        .value("WORLD_39", SDLK_WORLD_39)
        .value("WORLD_40", SDLK_WORLD_40)
        .value("WORLD_41", SDLK_WORLD_41)
        .value("WORLD_42", SDLK_WORLD_42)
        .value("WORLD_43", SDLK_WORLD_43)
        .value("WORLD_44", SDLK_WORLD_44)
        .value("WORLD_45", SDLK_WORLD_45)
        .value("WORLD_46", SDLK_WORLD_46)
        .value("WORLD_47", SDLK_WORLD_47)
        .value("WORLD_48", SDLK_WORLD_48)
        .value("WORLD_49", SDLK_WORLD_49)
        .value("WORLD_50", SDLK_WORLD_50)
        .value("WORLD_51", SDLK_WORLD_51)
        .value("WORLD_52", SDLK_WORLD_52)
        .value("WORLD_53", SDLK_WORLD_53)
        .value("WORLD_54", SDLK_WORLD_54)
        .value("WORLD_55", SDLK_WORLD_55)
        .value("WORLD_56", SDLK_WORLD_56)
        .value("WORLD_57", SDLK_WORLD_57)
        .value("WORLD_58", SDLK_WORLD_58)
        .value("WORLD_59", SDLK_WORLD_59)
        .value("WORLD_60", SDLK_WORLD_60)
        .value("WORLD_61", SDLK_WORLD_61)
        .value("WORLD_62", SDLK_WORLD_62)
        .value("WORLD_63", SDLK_WORLD_63)
        .value("WORLD_64", SDLK_WORLD_64)
        .value("WORLD_65", SDLK_WORLD_65)
        .value("WORLD_66", SDLK_WORLD_66)
        .value("WORLD_67", SDLK_WORLD_67)
        .value("WORLD_68", SDLK_WORLD_68)
        .value("WORLD_69", SDLK_WORLD_69)
        .value("WORLD_70", SDLK_WORLD_70)
        .value("WORLD_71", SDLK_WORLD_71)
        .value("WORLD_72", SDLK_WORLD_72)
        .value("WORLD_73", SDLK_WORLD_73)
        .value("WORLD_74", SDLK_WORLD_74)
        .value("WORLD_75", SDLK_WORLD_75)
        .value("WORLD_76", SDLK_WORLD_76)
        .value("WORLD_77", SDLK_WORLD_77)
        .value("WORLD_78", SDLK_WORLD_78)
        .value("WORLD_79", SDLK_WORLD_79)
        .value("WORLD_80", SDLK_WORLD_80)
        .value("WORLD_81", SDLK_WORLD_81)
        .value("WORLD_82", SDLK_WORLD_82)
        .value("WORLD_83", SDLK_WORLD_83)
        .value("WORLD_84", SDLK_WORLD_84)
        .value("WORLD_85", SDLK_WORLD_85)
        .value("WORLD_86", SDLK_WORLD_86)
        .value("WORLD_87", SDLK_WORLD_87)
        .value("WORLD_88", SDLK_WORLD_88)
        .value("WORLD_89", SDLK_WORLD_89)
        .value("WORLD_90", SDLK_WORLD_90)
        .value("WORLD_91", SDLK_WORLD_91)
        .value("WORLD_92", SDLK_WORLD_92)
        .value("WORLD_93", SDLK_WORLD_93)
        .value("WORLD_94", SDLK_WORLD_94)
        .value("WORLD_95", SDLK_WORLD_95)

        // Numpad
        .value("KP0", SDLK_KP0)
        .value("KP1", SDLK_KP1)
        .value("KP2", SDLK_KP2)
        .value("KP3", SDLK_KP3)
        .value("KP4", SDLK_KP4)
        .value("KP5", SDLK_KP5)
        .value("KP6", SDLK_KP6)
        .value("KP7", SDLK_KP7)
        .value("KP8", SDLK_KP8)
        .value("KP9", SDLK_KP9)
        .value("KP_PERIOD", SDLK_KP_PERIOD)
        .value("KP_DIVIDE", SDLK_KP_DIVIDE)
        .value("KP_MULTIPLY", SDLK_KP_MULTIPLY)
        .value("KP_MINUS", SDLK_KP_MINUS)
        .value("KP_PLUS", SDLK_KP_PLUS)
        .value("KP_ENTER", SDLK_KP_ENTER)
        .value("KP_EQUALS", SDLK_KP_EQUALS)

        // Home/end pad and arrows
        .value("UP", SDLK_UP)
        .value("DOWN", SDLK_DOWN)
        .value("RIGHT", SDLK_RIGHT)
        .value("LEFT", SDLK_LEFT)
        .value("INSERT", SDLK_INSERT)
        .value("HOME", SDLK_HOME)
        .value("END", SDLK_END)
        .value("PAGEUP", SDLK_PAGEUP)
        .value("PAGEDOWN", SDLK_PAGEDOWN)

        // Function keys
        .value("F1", SDLK_F1)
        .value("F2", SDLK_F2)
        .value("F3", SDLK_F3)
        .value("F4", SDLK_F4)
        .value("F5", SDLK_F5)
        .value("F6", SDLK_F6)
        .value("F7", SDLK_F7)
        .value("F8", SDLK_F8)
        .value("F9", SDLK_F9)
        .value("F10", SDLK_F10)
        .value("F11", SDLK_F11)
        .value("F12", SDLK_F12)
        .value("F13", SDLK_F13)
        .value("F14", SDLK_F14)
        .value("F15", SDLK_F15)

        // Key modifiers
        .value("NUMLOCK", SDLK_NUMLOCK)
        .value("CAPSLOCK", SDLK_CAPSLOCK)
        .value("SCROLLOCK", SDLK_SCROLLOCK)
        .value("RSHIFT", SDLK_RSHIFT)
        .value("LSHIFT", SDLK_LSHIFT)
        .value("RCTRL", SDLK_RCTRL)
        .value("LCTRL", SDLK_LCTRL)
        .value("RALT", SDLK_RALT)
        .value("LALT", SDLK_LALT)
        .value("RMETA", SDLK_RMETA)
        .value("LMETA", SDLK_LMETA)
        .value("LSUPER", SDLK_LSUPER)            // Left "Windows" key
        .value("RSUPER", SDLK_RSUPER)            // Right "Windows" key
        .value("MODE", SDLK_MODE)                // "Alt Gr" key 
        .value("COMPOSE", SDLK_COMPOSE)          // Multi-key compose key

        /// Miscellaneous function keys
        .value("HELP", SDLK_HELP)
        .value("PRINT", SDLK_PRINT)
        .value("SYSREQ", SDLK_SYSREQ)
        .value("BREAK", SDLK_BREAK)
        .value("MENU", SDLK_MENU)
        .value("POWER", SDLK_POWER)             // Power Macintosh power key 
        .value("EURO", SDLK_EURO)               // Some european keyboards
        .value("UNDO", SDLK_UNDO)               // Atari keyboard has Undo

        .export_values()
        ;

}

/*
 * Constructor
 *
 * @param Main_App* game Reference to the main application instance.
 */
Python_Interface::Python_Interface(Main_App* _game)
{
    game = _game;
}


/*
 * Starts up the python interpreter, creates the default
 * namespace and runs the main script.
 * 
 * @returns bool If an error was spit out by the python interpreter upon init.
 */
bool Python_Interface::initialise_python_interpreter()
{

    try
    {

        object main_module = import("__main__");
        object main_namespace = main_module.attr("__dict__");

        initcore();

        // Give the main app instance
		main_namespace["core"] = ptr(game);
		string const file_name = "logic/main.py";

        object ignored = exec_file(
            file_name.c_str(),
            main_namespace,
			main_namespace
            );

    }
    catch(error_already_set const &)
    {

        PyErr_Print();
        return False;

    }

    return True;

}
