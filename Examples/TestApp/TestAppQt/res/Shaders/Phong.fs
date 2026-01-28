#version 120

varying vec3 N;
varying vec3 v;
varying vec4 c;

uniform vec4  u_LightPosition;
uniform vec4  u_LightAmbient;
uniform vec4  u_LightDiffuse;
uniform vec4  u_LightSpecular;
uniform float u_LightShininess;

void main()
{  
    vec3 L = normalize(u_LightPosition.xyz - v);   
    vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
    vec3 R = normalize(-reflect(L,N));  
 
    //calculate Ambient Term:  
    vec4 Iamb = u_LightAmbient;

    //calculate Diffuse Term:  
    vec4 Idiff = u_LightDiffuse * max(dot(N,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);     
   
    // calculate Specular Term:
    vec4 Ispec = u_LightSpecular * pow(max(dot(R,E),0.0),u_LightShininess);
    Ispec = clamp(Ispec, 0.0, 1.0); 
   
    // write Total Color:  
    gl_FragColor = c * (Iamb + Idiff + Ispec);
}
