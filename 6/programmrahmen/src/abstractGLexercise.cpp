
#include <cassert>

#define GL_GLEXT_PROTOTYPES
#include <QtOpenGL>
#include <QBasicTimer>
#include <QTimerEvent>

#include "abstractglexercise.h"

#include "mathmacros.h"


namespace
{
    const int MSECS(16);   
}

AbstractGLExercise::AbstractGLExercise(QWidget * parent)
:   AbstractExercise()
,   QGLWidget(parent)
,   m_timer(new QBasicTimer())
,   m_frame(0)
{
    m_view.lookAt(
        QVector3D( 0.f, 1.f,-6.f)
    ,   QVector3D( 0.f, 0.f, 0.f)
    ,   QVector3D( 0.f, 1.f, 0.f));

    m_timer->start(MSECS, this);
	
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

AbstractGLExercise::~AbstractGLExercise()
{
    delete m_timer;
}

void AbstractGLExercise::timerEvent(QTimerEvent * event)
{
    assert(m_timer);

    if(event->timerId() != m_timer->timerId())
        return;

    if(m_active)
        ++m_frame;
    update();
}

void AbstractGLExercise::resizeGL(int width, int height)
{
    m_projection = QMatrix4x4();
    m_projection.perspective(
        60.f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 36.f);

    glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_projection.data());

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m_view.data());

    updateGL();
}

void AbstractGLExercise::initializeGL()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	
	static const float g_light0[]   = {-2.0f, 8.0f, 8.0f, 1.0f };
    static const float g_ambient0[] = { 0.8f, 0.8f, 1.0f, 1.0f };

    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, g_light0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, g_ambient0);

    glEnable(GL_CULL_FACE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    qDebug() << "OpenGL Context: " << this->context()->format().majorVersion() << "." << this->context()->format().minorVersion();
}

void AbstractGLExercise::enable()
{
    m_active = true;
}

void AbstractGLExercise::disable()
{
    m_active = false;
}




