#version 410

in vec4 v_vertex_colour[];
in vec2 v_texture_coord[];

out vec4 f_vertex_colour;
out vec2 f_texture_coord;

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

void main()
{	
  for(int i=0; i<gl_in.length(); i++)
  {
    f_vertex_colour = v_vertex_colour[i];
    f_texture_coord = v_texture_coord[i];
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
  }
  EndPrimitive();
}  

