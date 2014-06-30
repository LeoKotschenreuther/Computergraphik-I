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
    
	gl_FragColor = color;
}