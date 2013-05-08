varying vec2 f_texture_coord;
varying vec2 light_pos;

uniform sampler2D texture_num;
uniform vec2 screen_size;
uniform vec4 camera_position;
uniform float camera_zoom;
uniform float light_size;
uniform vec4 light_colour;

void main()
{

	gl_FragColor = texture2D(texture_num, f_texture_coord.xy);

    vec2 pos = vec2(f_texture_coord.x * screen_size.x, (1.0 - f_texture_coord.y) * screen_size.y);
    float distance = distance(light_pos, pos);

	if(distance < light_size)
    {
        float amount = ((1.0-min(distance/light_size, 1.0)) * light_colour.w) * gl_FragColor.a;
		gl_FragColor += (vec4(light_colour.x, light_colour.y, light_colour.z, 0.0) * amount);
	}

}