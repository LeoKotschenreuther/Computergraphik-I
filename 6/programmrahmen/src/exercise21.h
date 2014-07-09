#pragma once

#include "abstractGLexercise.h"

#include <math.h>
#include <string>
#include <vector>

#define SIZE 10 // max value: 10

class Exercise21 : public AbstractGLExercise
{
public:
    Exercise21(QWidget *parent = NULL);
    virtual ~Exercise21();

    virtual void paintGL();
    virtual void initializeGL();
    virtual const bool initialize();

    virtual const QString hints() const;

protected:
    virtual void keyPressEvent(QKeyEvent * keyEvent);
    void drawHeightFieldPoints();
    void drawHeightFieldLines();
    void drawTriangulatedHeightField();
    void drawHeightFieldBezierPatch();
    void drawGroundPlate();

    void initBezierLightingAndMaterial();

protected:
    void calculateHeightField();

    int m_gridSize;
    GLfloat m_heightField[SIZE][SIZE][3];
    bool m_togglePoints;
    bool m_toggleLines;
    bool m_toggleTriangles;
    bool m_toggleBezier;

    MaterialDefinition  m_bezierMaterial;
    LightingDefinition  m_lighting;
};
