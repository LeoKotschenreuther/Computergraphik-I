#include "exercise20.h"

#include <QKeyEvent>

#include "polygonaldrawable.h"
#include "halfedgestructure.h"
#include "vertexreuse.h"
#include "objio.h"

Exercise20::Exercise20(QWidget  * parent)
:   AbstractGLExercise(parent)
,   m_drawable(NULL)
,   m_he(NULL)
,   m_shadingMode(Phong_Shading)
,   m_materialMode(Gold)
{
    m_view = QMatrix4x4();
    m_view.lookAt(
        QVector3D( 0.f, 1.f,  4.f)
    ,   QVector3D( 0.f, 0.f,  0.f)
    ,   QVector3D( 0.f, 1.f,  0.f));

	m_prog_toon = new QGLShaderProgram;
	m_prog_phong = new QGLShaderProgram;

	setupLight();
    setupMaterials();
}

Exercise20::~Exercise20()
{
    delete m_materials;
    delete m_prog_toon;
	delete m_prog_phong;
    delete m_drawable;
    delete m_he;
}

const bool Exercise20::initialize()
{
    if(m_drawable)
        return true;

    m_drawable = ObjIO::fromObjFile("../data/suzanne.obj");
    
    VertexReuse::optimize(
        m_drawable->vertices()
    ,   m_drawable->normals()
    ,   m_drawable->texcs()
    ,   m_drawable->indices());

    m_he = new HalfEdgeStructure(*m_drawable);
    m_he->calculatePerVertexNormals(180.0f);

    return true;
}

void Exercise20::setupLight()
{
    m_lighting.m_pos[0] = 0.0f;
    m_lighting.m_pos[1] = 5.0f;
    m_lighting.m_pos[2] = 5.0f;
    m_lighting.m_pos[3] = 1.0f;
    m_lighting.m_iAmbient[0] = 0.2f;
    m_lighting.m_iAmbient[1] = 0.2f;
    m_lighting.m_iAmbient[2] = 0.2f;
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

void Exercise20::setupMaterials()
{
    m_materials = new MaterialDefinition[NumMaterialModes];
    // Gold
    m_materials[Gold].m_kAmbient[0] = 0.24725f;
    m_materials[Gold].m_kAmbient[1] = 0.1995f;
    m_materials[Gold].m_kAmbient[2] = 0.0745f;
    m_materials[Gold].m_kAmbient[3] = 1.0f;
    m_materials[Gold].m_kDiffuse[0] = 0.75164f;
    m_materials[Gold].m_kDiffuse[1] = 0.60648f;
    m_materials[Gold].m_kDiffuse[2] = 0.22648f;
    m_materials[Gold].m_kDiffuse[3] = 1.0f;
    m_materials[Gold].m_kSpecular[0] = 0.628281f;
    m_materials[Gold].m_kSpecular[1] = 0.555802f;
    m_materials[Gold].m_kSpecular[2] = 0.366065f;
    m_materials[Gold].m_kSpecular[3] = 1.0f;
    m_materials[Gold].m_kEmission[0] = 0.1f;
    m_materials[Gold].m_kEmission[1] = 0.1f;
    m_materials[Gold].m_kEmission[2] = 0.1f;
    m_materials[Gold].m_kEmission[3] = 0.0f;
    m_materials[Gold].m_shininess = 0.4f;

    // Red plastic
    m_materials[Red_Plastic].m_kAmbient[0] = 0.0f;
    m_materials[Red_Plastic].m_kAmbient[1] = 0.0f;
    m_materials[Red_Plastic].m_kAmbient[2] = 0.0f;
    m_materials[Red_Plastic].m_kAmbient[3] = 1.0f;
    m_materials[Red_Plastic].m_kDiffuse[0] = 0.5f;
    m_materials[Red_Plastic].m_kDiffuse[1] = 0.0f;
    m_materials[Red_Plastic].m_kDiffuse[2] = 0.0f;
    m_materials[Red_Plastic].m_kDiffuse[3] = 1.0f;
    m_materials[Red_Plastic].m_kSpecular[0] = 0.7f;
    m_materials[Red_Plastic].m_kSpecular[1] = 0.6f;
    m_materials[Red_Plastic].m_kSpecular[2] = 0.6f;
    m_materials[Red_Plastic].m_kSpecular[3] = 1.0f;
    m_materials[Red_Plastic].m_kEmission[0] = 0.1f;
    m_materials[Red_Plastic].m_kEmission[1] = 0.1f;
    m_materials[Red_Plastic].m_kEmission[2] = 0.1f;
    m_materials[Red_Plastic].m_kEmission[3] = 0.0f;
    m_materials[Red_Plastic].m_shininess = 0.25f;

    // Jade
    m_materials[Jade].m_kAmbient[0] = 0.135f;
    m_materials[Jade].m_kAmbient[1] = 0.2225f;
    m_materials[Jade].m_kAmbient[2] = 0.1575f;
    m_materials[Jade].m_kAmbient[3] = 1.0f;
    m_materials[Jade].m_kDiffuse[0] = 0.54f;
    m_materials[Jade].m_kDiffuse[1] = 0.89f;
    m_materials[Jade].m_kDiffuse[2] = 0.63f;
    m_materials[Jade].m_kDiffuse[3] = 1.0f;
    m_materials[Jade].m_kSpecular[0] = 0.316228f;
    m_materials[Jade].m_kSpecular[1] = 0.316228f;
    m_materials[Jade].m_kSpecular[2] = 0.316228f;
    m_materials[Jade].m_kSpecular[3] = 1.0f;
    m_materials[Jade].m_kEmission[0] = 0.1f;
    m_materials[Jade].m_kEmission[1] = 0.1f;
    m_materials[Jade].m_kEmission[2] = 0.1f;
    m_materials[Jade].m_kEmission[3] = 0.0f;
    m_materials[Jade].m_shininess = 0.1f;

    // Chrome
    m_materials[Chrome].m_kAmbient[0] = 0.25f;
    m_materials[Chrome].m_kAmbient[1] = 0.25f;
    m_materials[Chrome].m_kAmbient[2] = 0.25f;
    m_materials[Chrome].m_kAmbient[3] = 1.0f;
    m_materials[Chrome].m_kDiffuse[0] = 0.4f;
    m_materials[Chrome].m_kDiffuse[1] = 0.4f;
    m_materials[Chrome].m_kDiffuse[2] = 0.4f;
    m_materials[Chrome].m_kDiffuse[3] = 1.0f;
    m_materials[Chrome].m_kSpecular[0] = 0.774597f;
    m_materials[Chrome].m_kSpecular[1] = 0.774597f;
    m_materials[Chrome].m_kSpecular[2] = 0.774597f;
    m_materials[Chrome].m_kSpecular[3] = 1.0f;
    m_materials[Chrome].m_kEmission[0] = 0.1f;
    m_materials[Chrome].m_kEmission[1] = 0.1f;
    m_materials[Chrome].m_kEmission[2] = 0.1f;
    m_materials[Chrome].m_kEmission[3] = 0.0f;
    m_materials[Chrome].m_shininess = 0.6f;
}

void Exercise20::setupLightUniforms(QGLShaderProgram* prog)
{
	prog->setUniformValue("light_pos", QVector4D(m_lighting.m_pos[0], m_lighting.m_pos[1], m_lighting.m_pos[2], m_lighting.m_pos[3]));
	prog->setUniformValue("light_iAmbient", QVector4D(m_lighting.m_iAmbient[0], m_lighting.m_iAmbient[1], m_lighting.m_iAmbient[2], m_lighting.m_iAmbient[3]));
	prog->setUniformValue("light_iDiffuse", QVector4D(m_lighting.m_iDiffuse[0], m_lighting.m_iDiffuse[1], m_lighting.m_iDiffuse[2], m_lighting.m_iDiffuse[3]));
	prog->setUniformValue("light_iSpecular", QVector4D(m_lighting.m_iSpecular[0], m_lighting.m_iSpecular[1], m_lighting.m_iSpecular[2], m_lighting.m_iSpecular[3]));
}

void Exercise20::setupMaterialUniforms(QGLShaderProgram* prog)
{
	prog->setUniformValue("material_ambient", QVector4D(m_materials[m_materialMode].m_kAmbient[0], m_materials[m_materialMode].m_kAmbient[1], m_materials[m_materialMode].m_kAmbient[2], m_materials[m_materialMode].m_kAmbient[3]));
	prog->setUniformValue("material_diffuse", QVector4D(m_materials[m_materialMode].m_kDiffuse[0], m_materials[m_materialMode].m_kDiffuse[1], m_materials[m_materialMode].m_kDiffuse[2], m_materials[m_materialMode].m_kDiffuse[3]));
	prog->setUniformValue("material_specular", QVector4D(m_materials[m_materialMode].m_kSpecular[0], m_materials[m_materialMode].m_kSpecular[1], m_materials[m_materialMode].m_kSpecular[2], m_materials[m_materialMode].m_kSpecular[3]));
	prog->setUniformValue("material_emission", QVector4D(m_materials[m_materialMode].m_kEmission[0], m_materials[m_materialMode].m_kEmission[1], m_materials[m_materialMode].m_kEmission[2], m_materials[m_materialMode].m_kEmission[3]));
	prog->setUniformValue("material_shininess", m_materials[m_materialMode].m_shininess);
}

void Exercise20::initializeGL()
{
    AbstractGLExercise::initializeGL();
    glEnable(GL_NORMALIZE);

    loadToonProgram();
    loadPhongProgram();
}

void Exercise20::paintGL()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	QGLShaderProgram* currentProg;
	if (m_shadingMode == Toon_Shading)
	{
		currentProg = m_prog_toon;
	}
	else
	{
		currentProg = m_prog_phong;
	}

	currentProg->bind();

    setupLightUniforms(currentProg);
	setupMaterialUniforms(currentProg);

    glLoadMatrixf(m_view.data());
    
    glPushMatrix();

    glRotatef(static_cast<float>(m_frame % (4 * 360)) * 0.25f, 0.f, 1.f, 0.f);
    glScalef(1.f, 1.f, 1.f);

    draw();

    glPopMatrix();
	currentProg->release();
}

void Exercise20::draw()
{
    glEnable(GL_CULL_FACE);
    glBegin(GL_TRIANGLES);

    glColor3f(1.f, 1.f, 1.f);

    HalfEdgeStructure::t_faces::const_iterator i = m_he->faces().begin();
    const HalfEdgeStructure::t_faces::const_iterator iEnd = m_he->faces().end();
    for(; i != iEnd; ++i)
    {
        const HalfEdgeStructure::HalfEdge * const he0 = i->he->prev;
        const HalfEdgeStructure::HalfEdge * const he1 = i->he;
        const HalfEdgeStructure::HalfEdge * const he2 = i->he->next;

        glNormal3f(he0->normal.x(), he0->normal.y(), he0->normal.z());
        glVertex3f(he0->vertex->x(), he0->vertex->y(), he0->vertex->z());
        glNormal3f(he1->normal.x(), he1->normal.y(), he1->normal.z());
        glVertex3f(he1->vertex->x(), he1->vertex->y(), he1->vertex->z());
        glNormal3f(he2->normal.x(), he2->normal.y(), he2->normal.z());
        glVertex3f(he2->vertex->x(), he2->vertex->y(), he2->vertex->z());
    }
    glEnd();

    glPopMatrix();
}

const QString Exercise20::hints() const
{
    return "[R] : reload shaders. [1] : toon shading. [2]-[5] : phong shading with different materials (jade, chrome, red plastic, gold). [Space] : toggle rotation.";
}

void Exercise20::keyPressEvent(QKeyEvent* keyEvent)
{
    const int key = keyEvent->key();
    
    switch(key)
    {
        case Qt::Key_Space:
            isActive() ? disable() : enable();
            break;
        case Qt::Key_R:
            loadToonProgram();
            loadPhongProgram();
            break;
        case Qt::Key_1:
            m_shadingMode = Toon_Shading;
            break;
        case Qt::Key_2:
            m_shadingMode = Phong_Shading;
            m_materialMode = Jade;
            break;
        case Qt::Key_3:
            m_shadingMode = Phong_Shading;
            m_materialMode = Chrome;
            break;
        case Qt::Key_4:
            m_shadingMode = Phong_Shading;
            m_materialMode = Red_Plastic;
            break;
        case Qt::Key_5:
            m_shadingMode = Phong_Shading;
            m_materialMode = Gold;
            break;
        default:
            break;
    }

	AbstractGLExercise::keyPressEvent(keyEvent);
}

void Exercise20::loadToonProgram()
{
    m_prog_toon->removeAllShaders();
	m_prog_toon->addShaderFromSourceFile(QGLShader::Vertex, "../data/toon.vert");
	m_prog_toon->addShaderFromSourceFile(QGLShader::Fragment, "../data/toon.frag");
    m_prog_toon->link();
}

void Exercise20::loadPhongProgram()
{
    m_prog_phong->removeAllShaders();
	m_prog_phong->addShaderFromSourceFile(QGLShader::Vertex, "../data/phong.vert");
	m_prog_phong->addShaderFromSourceFile(QGLShader::Fragment, "../data/phong.frag");
    m_prog_phong->link();
}
