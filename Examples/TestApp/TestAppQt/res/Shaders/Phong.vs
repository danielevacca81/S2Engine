#version 120

attribute vec3 in_Vertex;
attribute vec4 in_Color;
attribute vec3 in_Normal;

varying vec3 N;
varying vec3 v;
varying vec4 c;

void main()  
{     
	v = vec3(gl_ModelViewMatrix * vec4(in_Vertex,1.0));
	N = normalize(gl_NormalMatrix * in_Normal);
	c = in_Color;

	gl_Position = gl_ModelViewProjectionMatrix * vec4(in_Vertex,1.0);
}