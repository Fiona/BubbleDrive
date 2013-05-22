varying vec2 f_texture_coord;

uniform sampler2D screen_texture_num;
uniform vec2 screen_size;
uniform sampler2D texture_num;
uniform float game_time; 

void main()
{

	float t = clamp(game_time / 6., 0., 1.);
 
vec2 coords = f_texture_coord.xy;
vec2 dir = coords - vec2(.5);
float dist = distance(coords, vec2(.5));
vec2 offset = (dir * (sin(dist * 50. ) + .5) / 30.) * .8;
 
vec2 texCoord = coords + offset;
vec4 diffuse = texture2D(texture_num, texCoord);
 
gl_FragColor = diffuse;

	//gl_FragColor = texture2D(texture_num, f_texture_coord.xy);


}