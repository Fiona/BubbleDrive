varying vec2 f_texture_coord;

uniform sampler2D screen_texture_num;
uniform sampler2D texture_num;
uniform vec2 screen_size;
uniform float blur_amount;

void main()
{

	float blur_size = blur_amount / (float)screen_size.x;

	vec4 sum = vec4(0.0);
 
	// take nine samples, with the distance blur_zize between them
	sum += texture2D(texture_num, vec2(f_texture_coord.x - (4.0 * blur_size), f_texture_coord.y)) * 0.05;
	sum += texture2D(texture_num, vec2(f_texture_coord.x - (3.0 * blur_size), f_texture_coord.y)) * 0.09;
	sum += texture2D(texture_num, vec2(f_texture_coord.x - (2.0 * blur_size), f_texture_coord.y)) * 0.12;
	sum += texture2D(texture_num, vec2(f_texture_coord.x - blur_size, f_texture_coord.y)) * 0.15;
	sum += texture2D(texture_num, vec2(f_texture_coord.x, f_texture_coord.y)) * 0.16;
	sum += texture2D(texture_num, vec2(f_texture_coord.x + blur_size, f_texture_coord.y)) * 0.15;
	sum += texture2D(texture_num, vec2(f_texture_coord.x + (2.0 * blur_size), f_texture_coord.y)) * 0.12;
	sum += texture2D(texture_num, vec2(f_texture_coord.x + (3.0 * blur_size), f_texture_coord.y)) * 0.09;
	sum += texture2D(texture_num, vec2(f_texture_coord.x + (4.0 * blur_size), f_texture_coord.y)) * 0.05;
 
	gl_FragColor = sum;

}