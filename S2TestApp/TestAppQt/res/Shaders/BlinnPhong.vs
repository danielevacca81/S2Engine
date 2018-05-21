#version 120

attribute vec3 in_Vertex;
attribute vec4 in_Color;
attribute vec3 in_Normal;

varying vec3 normalInterp;
varying vec3 vertPos;
varying vec4 color;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * vec4(in_Vertex,1.0);

    vec4 vertPos4 = gl_ModelViewMatrix * vec4(in_Vertex,1.0);
    vertPos       = vec3(vertPos4) / vertPos4.w;
    	
	normalInterp  = gl_NormalMatrix * in_Normal;
	color         = in_Color;
}