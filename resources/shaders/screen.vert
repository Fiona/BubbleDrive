attribute vec2 vertex_coord;
attribute vec4 vertex_colour;
attribute vec2 texture_coord;
attribute vec4 pos_rotation_scale;

varying vec4 f_colour;
varying vec2 f_texture_coord;

uniform sampler2D texture_num;
uniform sampler2D screen_texture_num;
uniform vec4 camera_position;
uniform vec2 screen_size;


/*
 * Matrix is responsible for rotating the object.
 */
float degrees_radians = radians(pos_rotation_scale.z);
mat4 rotation_matrix = mat4(
        vec4(cos(degrees_radians), sin(degrees_radians), 0.0, 0.0),
        vec4(-sin(degrees_radians), cos(degrees_radians), 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
);


/*
 * Responsible for scaling the object.
 */
mat4 scale_matrix = mat4(
        vec4(pos_rotation_scale.w, 0.0, 0.0, 0.0),
        vec4(0.0, pos_rotation_scale.w, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
);


void main()
{
      
	f_colour = vertex_colour;
	f_texture_coord = texture_coord;

    mat4 normalise_coordinate = mat4(
		vec4(1.0 / (screen_size.x * .5), 0.0, 0.0, 0.0), 
		vec4(0.0, -1.0 / (screen_size.y * .5), 0.0, 0.0), 
		vec4(0.0, 0.0, 1.0, 0.0),
		vec4(0.0, 0.0, 0.0, 1.0)
    );

    gl_Position = normalise_coordinate * 
    (
      vec4(
        pos_rotation_scale.x - (screen_size.x / 2),
        pos_rotation_scale.y - (screen_size.y / 2),
        0.0,
        0.0
      ) + (rotation_matrix * scale_matrix * vec4(vertex_coord.x, vertex_coord.y, 0.0, 1.0))
    );
      
}