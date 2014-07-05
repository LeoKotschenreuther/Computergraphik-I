#version 120

// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2014 - Aufgabenblatt 6
//                     - Aufgabe 20 (Toon)
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

uniform vec4 light_pos;

varying vec3 N;

void main()
{
    float intensity;
    vec4 baseColor = vec4(0.8,0.0,0.0,1.0);
    vec4 color = baseColor;
	
    vec3 normN = normalize(N);
	
    ///////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 20
    // Implement toon shading. Take the intensity and baseColor into account
    ///////////////////////////////////////////////////////////////////

    vec3 toLight = vec3(light_pos[0] - N.x, light_pos[1] - N.y, light_pos[2] - N.z);
    float lengthNormN = sqrt(normN.x * normN.x + normN.y * normN.y + normN.z * normN.z);
    float lengthToLight = sqrt(toLight[0] * toLight[0] + toLight[1] * toLight[1] + toLight[2] * toLight[2]);
    intensity = 1 - acos((toLight[0]*normN.x + toLight[1] * normN.y + toLight[2] * normN.z)/(lengthToLight * lengthNormN)) / 3.14159;
    if(intensity < 0){
        intensity = intensity * -1;
    }

    if(intensity < 0.1f){
        color = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else if(intensity > 0.0f && intensity <= 0.33){
        color.x = baseColor.x / 3;
        color.y = baseColor.y / 3;
        color.z = baseColor.z / 3;
        color.w = baseColor.w / 3;
    }
    else if(intensity > 0.33f && intensity <= 0.66){
        color.x = baseColor.x / 3 * 2;
        color.y = baseColor.y / 3 * 2;
        color.z = baseColor.z / 3 * 2;
        color.w = baseColor.w / 3 * 2;
    }
    else if(intensity > 0.66f && intensity <= 0.99){
        color.x = baseColor.x;
        color.y = baseColor.y;
        color.z = baseColor.z;
        color.w = baseColor.w;
    }
    else if(intensity > 0.99f){
        color = vec4(1.0, 1.0, 1.0, 1.0);
    }
    
    gl_FragColor = color;
}
