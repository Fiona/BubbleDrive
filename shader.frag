uniform sampler2D texture_num;

void main()
{
	gl_FragColor = texture2D(texture_num, gl_TexCoord[0].xy) * gl_Color;
}