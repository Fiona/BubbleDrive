#version 410

in vec4 f_vertex_colour;
in vec2 f_texture_coord;

out vec4 frag_colour;

uniform sampler2D texture_num;
uniform sampler2D screen_texture_num;

void main()
{
	frag_colour = texture2D(texture_num, f_texture_coord.xy) * f_vertex_colour;
}