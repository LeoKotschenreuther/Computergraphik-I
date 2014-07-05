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
// Matr.-Nr: 765530
// Matr.-Nr: 765853
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
    vec3 normN = normalize(N);
    vec3 L = vec3(light_pos.x - N.x, light_pos.y - N.y, light_pos.z - N.z);
    vec3 normL = normalize(L);
    float scalarNL = normL.x * normN.x + normL.y * normN.y + normL.z * normN.z;
    vec3 R = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i <= 2; i++){
        R[i] = 2 * normN[i] * scalarNL - normL[i];
    }
    float scalarRV = normalize(R.x * v.x + R.y * v.y + R.z * v.z);
    for(int i = 0; i <= 3; i++){
        resultColor[i] = light_iAmbient[i] * material_ambient[i];
        resultColor[i] += light_iDiffuse[i] * material_diffuse[i] * scalarNL;
        resultColor[i] += light_iSpecular[i] * material_specular[i] * pow(scalarRV, material_shininess);
    }

    gl_FragColor = clamp(resultColor,0.0,1.0);
}
