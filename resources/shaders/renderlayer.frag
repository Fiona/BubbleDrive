varying vec2 f_texture_coord;

uniform sampler2D texture_num;

void main()
{
	gl_FragColor = texture2D(texture_num, f_texture_coord.xy);
}