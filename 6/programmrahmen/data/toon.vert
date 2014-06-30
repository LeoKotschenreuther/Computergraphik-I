#version 120

varying vec3 N;

void main(void)
{
    N = normalize(gl_NormalMatrix * gl_Normal);
    
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}