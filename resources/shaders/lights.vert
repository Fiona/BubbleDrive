attribute vec2 vertex_coord;
attribute vec2 texture_coord;

varying vec2 f_texture_coord;
varying vec2 light_pos;

uniform sampler2D texture_num;
uniform vec2 screen_size;
uniform vec4 camera_position;
uniform float camera_zoom;
uniform vec2 light_coordinate;
uniform float light_size;
uniform vec4 light_colour;

void main()
{

	light_pos = vec2(light_coordinate.x, light_coordinate.y);
      
	f_texture_coord = texture_coord;
	gl_Position =  vec4(vertex_coord.x, vertex_coord.y, 0.0, 1.0);
	
}