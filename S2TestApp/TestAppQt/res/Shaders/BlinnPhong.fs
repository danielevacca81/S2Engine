#version 120
uniform vec4  u_LightPosition;
uniform vec4  u_LightAmbient;
uniform vec4  u_LightDiffuse;
uniform vec4  u_LightSpecular;
uniform float u_LightShininess;


varying vec3 normalInterp;
varying vec3 vertPos;
varying vec4 color;

void main()
{
    vec3 normal      = normalize(normalInterp);
    vec3 lightDir    = normalize(u_LightPosition.xyz - vertPos);
    float lambertian = max(dot(lightDir,normal), 0.0);
    float specular   = 0.0;

    vec3 viewDir = normalize(-vertPos);

    vec3 halfDir    = normalize(lightDir + viewDir);
    float specAngle = max(dot(halfDir, normal), 0.0);
    specular        = pow(specAngle, u_LightShininess);

    gl_FragColor = color * 
                   vec4(u_LightAmbient             +
                        u_LightDiffuse  * lambertian +
                        u_LightSpecular * specular);
}