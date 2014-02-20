attribute vec2 vertex_coord;
attribute vec2 texture_coord;

varying vec2 f_texture_coord;
varying vec2 shockwave_pos;

uniform sampler2D texture_num;
uniform vec2 screen_size;
uniform sampler2D shockwave_texture_num;
uniform vec2 shockwave_coordinate;
uniform float shockwave_size;
uniform float shockwave_alpha;

void main()
{

	shockwave_pos = vec2(shockwave_coordinate.x, shockwave_coordinate.y);
      
	f_texture_coord = texture_coord;
	gl_Position =  vec4(vertex_coord.x, vertex_coord.y, 0.0, 1.0);
	
}