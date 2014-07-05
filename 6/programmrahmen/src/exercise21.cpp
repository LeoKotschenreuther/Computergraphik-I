// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2014 - Aufgabenblatt 6
//                     - Aufgabe 21
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: 765 358
// Matr.-Nr: xxxxx
//
// ======================================

#include "exercise21.h"

#include <QKeyEvent>
#include <QtMath>
#include <QtGlobal>
#include <QVector3D>


Exercise21::Exercise21(QWidget  * parent)
:   AbstractGLExercise(parent),
    m_gridSize(SIZE*SIZE),
    m_toggleLines(false),
    m_togglePoints(true),
    m_toggleTriangles(false),
    m_toggleBezier(false)
{
    m_view = QMatrix4x4();
    m_view.lookAt(
        QVector3D( 0.f, 3.f,  5.f)
    ,   QVector3D( 0.f, 0.f,  0.f)
    ,   QVector3D( 0.f, 1.f,  0.f));

    calculateHeightField();
}

Exercise21::~Exercise21()
{
}

const bool Exercise21::initialize()
{
    initBezierLightingAndMaterial();
    return true;
}

void Exercise21::initBezierLightingAndMaterial()
{
    m_bezierMaterial.m_kAmbient[0] = 0.4f;
    m_bezierMaterial.m_kAmbient[1] = 0.0f;
    m_bezierMaterial.m_kAmbient[2] = 0.0f;
    m_bezierMaterial.m_kAmbient[3] = 1.0f;
    m_bezierMaterial.m_kDiffuse[0] = 0.7f;
    m_bezierMaterial.m_kDiffuse[1] = 0.0f;
    m_bezierMaterial.m_kDiffuse[2] = 0.0f;
    m_bezierMaterial.m_kDiffuse[3] = 1.0f;
    m_bezierMaterial.m_kSpecular[0] = 0.7f;
    m_bezierMaterial.m_kSpecular[1] = 0.6f;
    m_bezierMaterial.m_kSpecular[2] = 0.6f;
    m_bezierMaterial.m_kSpecular[3] = 1.0f;
    m_bezierMaterial.m_shininess = 0.25f;

    m_lighting.m_pos[0] = 0.0f;
    m_lighting.m_pos[1] = 0.0f;
    m_lighting.m_pos[2] = 0.2f;
    m_lighting.m_pos[3] = 1.0f;
    m_lighting.m_iAmbient[0] = 0.8f;
    m_lighting.m_iAmbient[1] = 0.8f;
    m_lighting.m_iAmbient[2] = 0.8f;
    m_lighting.m_iAmbient[3] = 1.0f;
    m_lighting.m_iDiffuse[0] = 1.0f;
    m_lighting.m_iDiffuse[1] = 1.0f;
    m_lighting.m_iDiffuse[2] = 1.0f;
    m_lighting.m_iDiffuse[3] = 1.0f;
    m_lighting.m_iSpecular[0] = 0.5f;
    m_lighting.m_iSpecular[1] = 0.5f;
    m_lighting.m_iSpecular[2] = 0.5f;
    m_lighting.m_iSpecular[3] = 1.0f;
}

void Exercise21::initializeGL()
{
   AbstractGLExercise::initializeGL();

   glEnable(GL_SMOOTH);
   glClearColor (0.0, 0.0, 0.0, 0.0);

   /////////////////////////////////////////////////////////////////////////////////////////////////
   // TODO: Aufgabe 21
   // Initialize a two-dimensional opengl evaluator.
   // Make sure that if m_gridSize changes, the evaluator still works
   /////////////////////////////////////////////////////////////////////////////////////////////////

//   glMap2f();
//   glEnable(GL_MAP2_VERTEX_3);
//   glMapGrid2f();

   glEnable(GL_AUTO_NORMAL);
   glEnable(GL_NORMALIZE);
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_LIGHTING);
}

void Exercise21::calculateHeightField()
{
    int sqrtGridSize = qSqrt(m_gridSize);
    for (int i = 0; i < sqrtGridSize; i++)
    {
        float y = 2.0f - i*(4.0f / (sqrtGridSize-1));
        for (int j = 0; j < sqrtGridSize; j++)
        {
            float x = -2.0f + j*(4.0f / (sqrtGridSize-1));

            m_heightField[i][j][0] = x;
            float height = 0.01f + qrand() % 5 / 5.0f;
            m_heightField[i][j][1] = height;
            m_heightField[i][j][2] = -1*y;
        }
    }
}


void Exercise21::paintGL()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glLoadMatrixf(m_view.data());

    glPushMatrix();

    glRotatef(static_cast<float>(m_frame % (4 * 360)) * 0.25f, 0.f, 1.f, 0.f);
    glScalef(1.f, 1.f, 1.f);

    drawGroundPlate();

    if (m_toggleBezier)
        drawHeightFieldBezierPatch();
    if (m_toggleTriangles)
        drawTriangulatedHeightField();
    if (m_toggleLines)
        drawHeightFieldLines();
    if (m_togglePoints)
        drawHeightFieldPoints();

    glPopMatrix();
}

void Exercise21::drawGroundPlate()
{
    glPushMatrix();
    glBegin(GL_LINE_LOOP);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-2.5f, 0.0f, -2.5f);
    glVertex3f(2.5f, 0.0f, -2.5f);
    glVertex3f(2.5f, 0.0f, 2.5f);
    glVertex3f(-2.5f, 0.0f, 2.5f);

    glEnd();
    glPopMatrix();
}

void Exercise21::drawHeightFieldPoints()
{
    glPushMatrix();

    glPointSize(10);
    glBegin(GL_POINTS);

    glColor3f(1.f, 1.f, 1.f);

    int sqrtGridSize = qSqrt(m_gridSize);
    for (int i = 0; i < sqrtGridSize; i++)
    {
        for (int j = 0; j < sqrtGridSize; j++)
        {
            /////////////////////////////////////////////////////////////////////////////////////////////////
            // TODO: Aufgabe 21
            // Visualize the height field using GL_POINTS.
            /////////////////////////////////////////////////////////////////////////////////////////////////
            GLfloat pointVectorX = m_heightField [i] [j] [0];
            GLfloat pointVectorY = m_heightField [i] [j] [1];
            GLfloat pointVectorZ = m_heightField [i] [j] [2];
            glVertex3f(pointVectorX, pointVectorY, pointVectorZ);
        }
    }
    glEnd();
    glPopMatrix();
}

void Exercise21::drawHeightFieldLines()
{
    glPushMatrix();
    glBegin(GL_LINES);

    glColor3f(1.f, 1.f, 0.f);
    int sqrtGridSize = qSqrt(m_gridSize);

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 21
    // Visualize the height field using GL_LINES.
    // Make sure that if m_gridSize changes, the rendering results are still correct.
    /////////////////////////////////////////////////////////////////////////////////////////////////

    for (int i = 0; i < sqrtGridSize; i++)
    {
        for (int j = 0; j < sqrtGridSize; j++)
        {
            GLfloat pointVectorX = m_heightField [i] [j] [0];
            GLfloat pointVectorY = m_heightField [i] [j] [1];
            GLfloat pointVectorZ = m_heightField [i] [j] [2];
            glVertex3f(pointVectorX, pointVectorY, pointVectorZ);
            glVertex3f(pointVectorX, 0, pointVectorZ);
        }
    }

    glEnd();
    glPopMatrix();
}

void Exercise21::drawTriangulatedHeightField()
{
    glDisable(GL_CULL_FACE);
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.f, 1.f, 1.f);

    int direction = 1; //rechts, -1 wäre nach links

    int sqrtGridSize = qSqrt(m_gridSize);

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 21
    // Triangulate the given height field. Use GL_TRIANGLE_STRIPS.
    // Make sure that if m_gridSize changes, the triangulation still works.
    /////////////////////////////////////////////////////////////////////////////////////////////////

    for (int i = 0; i < sqrtGridSize; i++)
    {
        for (int j = 0; j < sqrtGridSize; j++)
        {
            int m_j;
            if (direction < 0) {
                m_j = sqrtGridSize - j;
            } else {
                m_j = j;
            }
            GLfloat pointVectorX = m_heightField [i] [m_j] [0];
            GLfloat pointVectorY = m_heightField [i] [m_j] [1];
            GLfloat pointVectorZ = m_heightField [i] [m_j] [2];
            glVertex3f(pointVectorX, pointVectorY, pointVectorZ);

            if (i+1 < sqrtGridSize) {
                glVertex3f(m_heightField [i+1] [m_j] [0], m_heightField [i+1] [m_j] [1], m_heightField [i+1] [m_j] [2]);
            } else {
                glVertex3f(m_heightField [i] [m_j] [0], m_heightField [i] [m_j] [1], m_heightField [i] [m_j] [2]);
            }
        }
        if (direction < 0) {
            direction = 1;
        } else {
            direction = -1;
        }
    }
    glEnd();

    glPopMatrix();
    glEnable(GL_CULL_FACE);
}

void Exercise21::drawHeightFieldBezierPatch()
{
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_lighting.m_iAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lighting.m_iDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, m_lighting.m_iSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, m_lighting.m_pos);

    glMaterialfv(GL_FRONT, GL_AMBIENT, m_bezierMaterial.m_kAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_bezierMaterial.m_kDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_bezierMaterial.m_kSpecular);
    GLfloat sh[] = {m_bezierMaterial.m_shininess};
    glMaterialfv(GL_FRONT, GL_SHININESS, sh);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glPushMatrix ();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glColor3f(1.f, 1.f, 1.f);

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 21
    // Draw a *filled* bezier patch using the opengl evaluator
    /////////////////////////////////////////////////////////////////////////////////////////////////
    //glEvalMesh2();

    glPopMatrix ();
    glDisable(GL_LIGHTING);
}

const QString Exercise21::hints() const
{
    return "[P] : toggle points. [L] : toggle lines. [T] : toggle triangulation. [B] : toggle bezier patch. [Space] : toggle rotation.";
}

void Exercise21::keyPressEvent(QKeyEvent* keyEvent)
{
    const int key = keyEvent->key();

    switch(key)
    {
        case Qt::Key_Space:
            isActive() ? disable() : enable();
            break;
        case Qt::Key_P:
            m_togglePoints = !m_togglePoints;
            break;
        case Qt::Key_L:
            m_toggleLines = !m_toggleLines;
        break;
        case Qt::Key_T:
            m_toggleTriangles = !m_toggleTriangles;
        break;
        case Qt::Key_B:
            m_toggleBezier = !m_toggleBezier;
        break;
        default:
            break;
    }

    AbstractGLExercise::keyPressEvent(keyEvent);
}
