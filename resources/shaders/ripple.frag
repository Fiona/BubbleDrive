varying vec2 f_texture_coord;
varying vec2 shockwave_pos;

uniform vec2 screen_size;
uniform sampler2D texture_num;
uniform sampler2D shockwave_texture_num;
uniform float shockwave_size;
uniform float shockwave_alpha;

float shockwave_intensity = 50.0;

void main()
{

    vec4 diffuse = texture2D(texture_num, f_texture_coord.xy);
    gl_FragColor = diffuse;

    vec2 frag_pos_in_pixels = vec2(f_texture_coord.x * screen_size.x, (1.0 - f_texture_coord.y) * screen_size.y);

    if(frag_pos_in_pixels.x >= (shockwave_pos.x - shockwave_size) && 
       frag_pos_in_pixels.x <= (shockwave_pos.x + shockwave_size) && 
       frag_pos_in_pixels.y >= (shockwave_pos.y - shockwave_size) && 
       frag_pos_in_pixels.y <= (shockwave_pos.y + shockwave_size))
    {
/*
        float scale = (shockwave_size / screen_size.x);

        vec2 ripple_tex_pos = vec2(
          f_texture_coord.x - ((shockwave_pos.x / screen_size.x) * scale),
          f_texture_coord.y - ((shockwave_pos.y / screen_size.y) * scale)
        );
*/
        vec2 adjusted_shockwave_frag_pos = ((((frag_pos_in_pixels - shockwave_pos) / shockwave_size) / 2.0) + vec2(.5, .5));
        vec4 shockwave_frag = texture2D(shockwave_texture_num, adjusted_shockwave_frag_pos.xy);
        float displacement = (shockwave_frag.r * -shockwave_intensity) * shockwave_alpha;

        vec2 direction_and_offset = normalize(frag_pos_in_pixels - shockwave_pos) * displacement;

        vec2 frag_pos_to_read = vec2(
            (frag_pos_in_pixels.x + direction_and_offset.x) / screen_size.x, 
            -((frag_pos_in_pixels.y + direction_and_offset.y) / screen_size.y)
        );

        gl_FragColor = texture2D(texture_num, frag_pos_to_read);

	}

}