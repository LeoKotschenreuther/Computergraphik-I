
#pragma once

#include "abstractGLexercise.h"

#include <math.h>
#include <string>
#include <vector>

//class Model;
class PolygonalDrawable;
class HalfEdgeStructure;

class Exercise18 : public AbstractGLExercise
{
public:
    Exercise18(QWidget *parent = NULL);
    virtual ~Exercise18();

    virtual void paintGL();
	virtual const bool initialize();

    virtual const QString hints() const;

protected:
	virtual void keyPressEvent(QKeyEvent * keyEvent);

	void draw();

    void drawFaceNormals();
    void drawVertexNormals();

private:
    PolygonalDrawable * m_drawable;
    HalfEdgeStructure * m_he;

    float m_threshold;
    bool m_normals;
};
