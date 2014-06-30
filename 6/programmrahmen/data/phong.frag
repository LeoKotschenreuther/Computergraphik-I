#version 120

// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2014 - Aufgabenblatt 6
//                     - Aufgabe 20 (Phong)
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

uniform vec4 light_pos;
uniform vec4 light_iAmbient;
uniform vec4 light_iDiffuse;
uniform vec4 light_iSpecular;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform vec4 material_emission;
uniform float material_shininess;

varying vec3 N;
varying vec3 v;

void main (void)
{
    ///////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 20
    // Implement phong shading.
    ///////////////////////////////////////////////////////////////////
    
    // write Total Color:
    vec4 resultColor = vec4(0.0,0.0,0.0,1.0);

    gl_FragColor = clamp(resultColor,0.0,1.0);
}
