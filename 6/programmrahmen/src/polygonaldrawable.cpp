
#include <cassert>

#include "polygonaldrawable.h"


PolygonalDrawable::PolygonalDrawable()
{
}

PolygonalDrawable::~PolygonalDrawable()
{
}

void PolygonalDrawable::draw(
    GLuint & list
,   const QMatrix4x4 & transform)
{
    if(-1 != list)
    {
        glCallList(list);
        return;
    }

    list = glGenLists(1);

    glNewList(list, GL_COMPILE_AND_EXECUTE);
    draw(transform);
    glEndList();
}

void PolygonalDrawable::draw(
    const QMatrix4x4 & transform)
{
    glPushMatrix();
    glMultMatrixf(transform.data());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glBegin(m_mode);

        t_uints::const_iterator i = m_indices.begin();
        const t_uints::const_iterator iEnd = m_indices.end();

        for(; i != iEnd; ++i)
        {
            if(!m_texcs.isEmpty())
                glTexCoord2f(m_texcs[*i].x(), m_texcs[*i].y());
            if(!m_normals.isEmpty())
                glNormal3f(m_normals[*i].x(), m_normals[*i].y(), m_normals[*i].z());

            glVertex3f(m_vertices[*i].x(), m_vertices[*i].y(), m_vertices[*i].z());
        }

    glEnd();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glPopMatrix();
}

const GLenum PolygonalDrawable::mode() const
{
    return m_mode;
}

void PolygonalDrawable::setMode(const GLenum mode)
{
    m_mode = mode;
}

const t_vec3s & PolygonalDrawable::vertices() const
{
    return m_vertices;
}

t_vec3s & PolygonalDrawable::vertices()
{
    return m_vertices;
}

const t_vec3s & PolygonalDrawable::normals() const
{
    return m_normals;
}

t_vec3s & PolygonalDrawable::normals()
{
    return m_normals;
}

const t_vec2s & PolygonalDrawable::texcs() const
{
    return m_texcs;
}

t_vec2s & PolygonalDrawable::texcs()
{
    return m_texcs;
}

const t_uints & PolygonalDrawable::indices() const
{
    return m_indices;
}

t_uints & PolygonalDrawable::indices()
{
    return m_indices;
}

void PolygonalDrawable::retrieveNormals()
{
    assert(m_indices.size() % 3 == 0);

    unsigned int i0, i1, i2;
    QVector3D v[3], a, b, n;

    unsigned int m(0);
    for(int i = 0; i < m_indices.size(); ++i)
        if(m_indices[i] > m)
            m = m_indices[i];

    if(m_normals.size() < static_cast<int>(m))
        m_normals.resize(m + 1);

    for(int i = 0; i < m_indices.size(); i += 3)
    {
        i0 = m_indices[i + 0];
        i1 = m_indices[i + 1];
        i2 = m_indices[i + 2];

        a = (m_vertices[i2] - m_vertices[i0]).normalized();
        b = (m_vertices[i1] - m_vertices[i0]).normalized();

        n = a * b;

        m_normals[i0] = n;
        m_normals[i1] = n;
        m_normals[i2] = n;
    }
}