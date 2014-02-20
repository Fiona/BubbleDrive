attribute vec2 vertex_coord;
attribute vec2 texture_coord;

varying vec2 f_texture_coord;

uniform sampler2D texture_num;
uniform vec2 screen_size;

void main()
{
      
	f_texture_coord = texture_coord;
    gl_Position =  vec4(vertex_coord.x, vertex_coord.y, 0.0, 1.0);	
    gl_TexCoord[0] = gl_MultiTexCoord0;

}