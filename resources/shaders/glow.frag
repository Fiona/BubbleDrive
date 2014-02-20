varying vec2 f_texture_coord;

uniform sampler2D screen_texture_num;
uniform sampler2D texture_num;
uniform vec2 screen_size;

void main()
{

  vec4 sum = vec4(0);
  vec2 texcoord = f_texture_coord.st;
  int j;
  int i;

  for( i= -4 ;i < 4; i++)
  {
    for (j = -3; j < 3; j++)
    {
      sum += texture2D(texture_num, texcoord + vec2(j, i)*0.004) * 0.25;            
    }
  }
  if (texture2D(texture_num, texcoord).r < 0.3)
  {
    gl_FragColor = sum*sum*0.012 + texture2D(texture_num, texcoord);
  }
  else
  {
    if (texture2D(texture_num, texcoord).r < 0.5)
    {
      gl_FragColor = sum*sum*0.009 + texture2D(texture_num, texcoord);
    }
    else
    {
      gl_FragColor = sum*sum*0.0075 + texture2D(texture_num, texcoord);        
    }
  }

}