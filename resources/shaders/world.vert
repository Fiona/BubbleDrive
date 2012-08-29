attribute vec2 vertex_coord;
attribute vec4 vertex_colour;
attribute vec2 texture_coord;
attribute vec4 pos_rotation_scale;

varying vec4 f_colour;
varying vec2 f_texture_coord;

uniform sampler2D texture_num;

uniform vec4 camera_position;
uniform vec2 screen_size;

/*
 * This matrix applies perspective projection to coordinates.
 */
float angle_of_view = radians(30.0); // half fov
float aspect_ratio = screen_size.x / screen_size.y;
float z_near = 0.5;
float z_far = 3001.0;
mat4 frustrum_matrix = mat4(
  vec4(1.0/tan(angle_of_view),           0.0, 0.0, 0.0),
  vec4(0.0, -aspect_ratio/tan(angle_of_view),  0.0, 0.0),
  vec4(0.0, 0.0,    (z_far+z_near)/(z_far-z_near), 1.0),
  vec4(0.0, 0.0, -2.0*z_far*z_near/(z_far-z_near), 0.0)
);


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
      
    gl_Position = frustrum_matrix * 
    (
      vec4(
        pos_rotation_scale.x - camera_position.x,
        pos_rotation_scale.y - camera_position.y,
        camera_position.z,
        0.0
      ) + (rotation_matrix * scale_matrix * vec4(vertex_coord.x, vertex_coord.y, 0.0, 1.0))
    );
}