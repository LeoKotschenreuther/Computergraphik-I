
#pragma once

#include "abstractglexercise.h"

#include <vector>
#include <list>

struct Vertex 
{
	double x;
    double y; 
    double z;
	
    Vertex(
        const double x
    ,   const double y
    ,   const double z)
    :   x(x)
    ,   y(y)
    ,   z(z)
    {
    }
};

typedef std::vector<Vertex>  Contour;
typedef std::vector<Contour> ContourList;

class Exercise19 : public AbstractGLExercise
{
public:
    Exercise19(QWidget * parent = NULL);
    virtual ~Exercise19()
    {
    };

	// from abstractglexercise.h
	virtual void paintGL();
	virtual void initializeGL();
    virtual void resizeGL(int w, int h);

	// from abstractexercise.h
	virtual const bool initialize();

	virtual const QString hints() const;
	virtual const QString description() const;

	virtual void mouseReleaseEvent(QMouseEvent * mouseEvent);
    virtual void keyPressEvent(QKeyEvent * event);


protected:
	void drawContours();
	void tesselatePolygons();
	
	ContourList m_contours;

};
