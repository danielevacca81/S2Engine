#version 120

attribute vec3 in_Vertex;
attribute vec4 in_Color;

void main()
{
	gl_Position   = gl_ModelViewProjectionMatrix * vec4(in_Vertex,1);
	gl_FrontColor = in_Color;
}