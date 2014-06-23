
#include "exercise19.h"
#include "mathmacros.h"

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif


#include <iostream>
#include <QtGlobal>
#include <QMouseEvent>
#include <QWindow>


// global state FTW

std::list<Vertex> g_combinedVertices; // new vertices from tesselation (see combineCallback)
Contour g_vertices; // vertices for the current primitive

GLenum g_primitive; // Current primitive type


void errorCallback(GLenum errorCode)
{
	const GLubyte * estring;
	estring = gluErrorString(errorCode);

    qWarning() << "Tessellation Error: " << estring;
}

void beginCallback(GLenum primitive)
{
    glBegin(primitive);
    g_primitive = primitive;
    g_vertices.clear();
}

void vertexCallback(void * vdata)
{
    double* vertexdata = static_cast<double*>(vdata);
    Vertex newVertex(vertexdata[0], vertexdata[1], vertexdata[2]);
    g_vertices.push_back(newVertex);
}

void combineCallback(double coords[3], double *vertex_data[4], float weight[4], double **dataOut)
{
	Vertex vertex(coords[0], coords[1], coords[2]);
	g_combinedVertices.push_back(vertex);
	Vertex &v = g_combinedVertices.back();
	*dataOut = &v.x;
}

void endCallback(void)
{
    glDisable(GL_LIGHTING);

        // Draw solid

        glColor3f(0.7, 0.7, 0.7);

        glBegin(g_primitive);

        for(unsigned int i=0; i<g_vertices.size(); i++) {
            Vertex v = g_vertices[i];
            glVertex3f(v.x, v.y, v.z);
        }

        glEnd();


        // Draw triangle outlines

        glColor3f(0.9, 0.9, 0.9);
        glLineWidth(1.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBegin(g_primitive);

        for(unsigned int i=0; i<g_vertices.size(); i++) {
            Vertex v = g_vertices[i];
            glVertex3f(v.x, v.y, v.z + 0.01); // + 0.01 so it's guaranteed to be in front of the solid
        }

        glEnd();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



Exercise19::Exercise19(QWidget* parent)
:   AbstractGLExercise(parent)
{
}
void Exercise19::paintGL()
{
    float ratio = windowHandle()->devicePixelRatio();
    glViewport(0, 0, (GLsizei) (width()) *ratio, (GLsizei) (height() *ratio));


	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glPushMatrix();

    //TODO use these
    drawContours();
    tesselatePolygons();

	glPopMatrix();
}

const bool Exercise19::initialize()
{
	m_contours.push_back(Contour());
	return true;
}

const QString Exercise19::hints() const
{
	return "Left click to add point to current contour. Right click to close current contour.";
}

const QString Exercise19::description() const
{
	return "Tesselation";
}

void Exercise19::drawContours()
{
    static const GLenum mode[] = { GL_POINTS, GL_LINES, GL_POLYGON };

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (unsigned int i = 0; i < m_contours.size(); ++i) 
	{
		const Contour contour = m_contours[i];

        const QColor color((Qt::GlobalColor)(i % 13 + 6));
		glColor3f(color.redF(), color.greenF(), color.blueF());

		if (contour.size() > 0) 
		{
            int modeIndex = _mi(contour.size()-1,2);
            glBegin(mode[modeIndex]);

			for (Contour::const_iterator it = contour.begin(); it != contour.end(); ++it) 
            {
                qDebug() << it->x << ", " << it->y;
				glVertex3f(it->x, it->y, it->z);
            }
			glEnd();
		}
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Exercise19::tesselatePolygons()
{
    GLUtesselator *tess = gluNewTess();

        gluTessCallback(tess, GLU_TESS_ERROR,   (GLvoid(*)()) &errorCallback);
        gluTessCallback(tess, GLU_TESS_BEGIN,   (GLvoid(*)()) &beginCallback);
        gluTessCallback(tess, GLU_TESS_END,     (GLvoid(*)()) &endCallback);
        gluTessCallback(tess, GLU_TESS_VERTEX,  (GLvoid(*)()) &vertexCallback);
        gluTessCallback(tess, GLU_TESS_COMBINE, (GLvoid(*)()) &combineCallback);

        gluTessBeginPolygon(tess, NULL);

        for (ContourList::iterator it = m_contours.begin(); it != m_contours.end(); ++it)
        {
            Contour & contour = *it;
            if (contour.size() <= 2)
                continue;

            gluTessBeginContour(tess);
            for (Contour::iterator cit = contour.begin(); cit != contour.end(); ++cit)
                gluTessVertex(tess, &(cit->x), &(cit->x));

            gluTessEndContour(tess);
        }

        gluTessEndPolygon(tess);
        gluDeleteTess(tess);

        // Release newly allocated vertices
        g_combinedVertices.clear();
}

void Exercise19::mouseReleaseEvent(QMouseEvent * mouseEvent)
{
	if (mouseEvent->button() == Qt::LeftButton) 
	{ 
        // Add vertex to contour
        float ratio = windowHandle()->devicePixelRatio();
        int x = mouseEvent->pos().x()*ratio;
        int y = mouseEvent->pos().y()*ratio;

		Vertex v(static_cast<float>(x), static_cast<float>(y), 0.f);
		m_contours[m_contours.size() - 1].push_back(v);
	}

	else if (mouseEvent->button() == Qt::RightButton) 
	{   
        // Finish current contour and go to the next one
		Contour & current = m_contours[m_contours.size() - 1];

		if (current.size() >= 3) 
        {
			// The last contour has 3 or more vertices, so it's valid and we create a new one
			Contour contour;
			m_contours.push_back(contour);
		}
        else
        {
			// The last contour did not have 3 or more vertices, so we clear it and start again
			current.clear();
		}
	}

	updateGL();
}

void Exercise19::keyPressEvent(QKeyEvent * event)
{
	switch(event->key())
    {
    case Qt::Key_R:

        g_vertices.clear();
        g_combinedVertices.clear();

        m_contours.clear();

        m_contours.push_back(Contour());

        updateGL();
        break;
    }
}

void Exercise19::resizeGL(int w, int h)
{
    if (w > 0 && h > 0)
    {

        glViewport(0, 0, (GLsizei)w, (GLsizei)h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w, h, 0, -1.0l, 1.0l);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
}

void Exercise19::initializeGL()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);

    glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);

    glPointSize(2.f);
}
