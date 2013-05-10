varying vec2 f_texture_coord;

uniform sampler2D screen_texture_num;
uniform sampler2D texture_num;
uniform float saturation_amount; 

void main()
{

	vec4 out_col = texture2D(texture_num, f_texture_coord.xy);
	
	float p = sqrt(
		out_col.r * out_col.r * .299 + 
		out_col.g * out_col.g * .587 + 
		out_col.b * out_col.b * .114
		);

	gl_FragColor = vec4(
		p + (out_col.r - p) * saturation_amount,
		p + (out_col.g - p) * saturation_amount,
		p + (out_col.r - p) * saturation_amount,
		out_col.a
		);

}