uniform sampler2D texture_num;
uniform vec4 position;
uniform float rotation;
uniform float scale;
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
float degrees_radians = radians(rotation);
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
        vec4(scale, 0.0, 0.0, 0.0),
        vec4(0.0, scale, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
);


void main()
{
	gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = frustrum_matrix *
      (
        vec4(
          (position.x - camera_position.x),
          (position.y - camera_position.y),
          camera_position.z,
          0.0
        ) + (rotation_matrix * scale_matrix * gl_Vertex)
      );
}