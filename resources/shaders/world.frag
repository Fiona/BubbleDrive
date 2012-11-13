varying vec4 f_colour;
varying vec2 f_texture_coord;

uniform sampler2D texture_num;
uniform sampler2D screen_texture_num;

void main()
{
	//gl_FragColor = mix(texture2D(screen_texture_num, vec2(gl_TexCoord[1])), texture2D(texture_num, f_texture_coord.xy), 0.5) * f_colour;
	gl_FragColor = texture2D(texture_num, f_texture_coord.xy) * f_colour;
	//gl_FragColor = texture2D(texture_num, f_texture_coord.xy) * f_colour;
}