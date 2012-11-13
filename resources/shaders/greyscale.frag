varying vec2 f_texture_coord;

uniform sampler2D screen_texture_num;
uniform sampler2D texture_num;

const float blurSizeH = 1.0 / 1000.0;
const float blurSizeV = 1.0 / 1000.0;

void main()
{

	vec4 out_col = texture2D(texture_num, f_texture_coord.xy);
	float avg = 0.2126 * out_col.r + 0.7152 * out_col.g + 0.0722 * out_col.b;
	gl_FragColor = vec4( avg, avg, avg, out_col.a);
	
}