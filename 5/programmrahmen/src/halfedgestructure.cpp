
#include <cassert>

#include "halfedgestructure.h"
#include "polygonaldrawable.h"
#include "mathmacros.h"


HalfEdgeStructure::HalfEdgeStructure(PolygonalDrawable & drawable)
{
    setup(drawable);
}

HalfEdgeStructure::~HalfEdgeStructure()
{
}

const HalfEdgeStructure::t_halfEdges & HalfEdgeStructure::halfEdges() const
{
    return m_halfEdges;
}

const HalfEdgeStructure::t_faces & HalfEdgeStructure::faces() const
{
    return m_faces;
}

void HalfEdgeStructure::setup(PolygonalDrawable & drawable)
{
    if(drawable.indices().isEmpty())
		return;

    const int nTriangles(drawable.indices().size() / 3);
    m_faces.resize(nTriangles);

    // TODO: there is strict triangle support only...
    assert(drawable.indices().size() % 3 == 0);
  
	// Map of edges (given by two indices) -> opposite half-edge

    typedef std::map<std::pair<QVector3D const *, QVector3D const *>, HalfEdge*> t_opp_map;
    t_opp_map opp;

    const int he_count = nTriangles * 3;
	m_halfEdges.resize(he_count);

	for(int k = 0; k < nTriangles; k++) 
	{
        m_faces[k].he = &m_halfEdges[k * 3];

		for (int i = 0; i < 3; ++i) 
		{
            const int j(k * 3 + i);

            m_halfEdges[j].prev = &m_halfEdges[(i == 0) ? j + 2 : j - 1];
			m_halfEdges[j].next = &m_halfEdges[(i == 2) ? j - 2 : j + 1];
			m_halfEdges[j].opp = NULL;

            m_halfEdges[j].face = &m_faces[k];

            const int l(drawable.indices()[j]);

            m_halfEdges[j].vertex = &(drawable.vertices()[l]);
            m_halfEdges[j].normal =   drawable.normals()[l];
        }

        // set opposite-pointers

        for (int i = 0; i < 3; ++i) 
		{
            const int j(k * 3 + i);

			QVector3D const * v0 = m_halfEdges[j].vertex;
			QVector3D const * v1 = m_halfEdges[j].next->vertex;

			// Check if opposite half-edge is already stored
			t_opp_map::iterator p = opp.find(t_opp_map::key_type(v0, v1));
			if(p == opp.end()) 
			{   // no: Add half-edge in opposite direction
				opp[t_opp_map::key_type(v1, v0)] = &m_halfEdges[j];
			} 
			else 
			{
				// yes: Set opposite-pointers of both half-edges
				p->second->opp = &m_halfEdges[j];
                m_halfEdges[j].opp = p->second;

				opp.erase(p);
            }
        }
    }

    calculatePerFaceNormals();
    calculatePerVertexNormals(0.f);
}

void HalfEdgeStructure::calculatePerFaceNormals()
{
    HalfEdgeStructure::t_faces::iterator i = m_faces.begin();
    const HalfEdgeStructure::t_faces::const_iterator iEnd = m_faces.end();

    for(; i != iEnd; ++i)
	{
        // calc face normal
        // TODO
        QVector3D v0 = i->he->vertex[0];
        QVector3D v1 = i->he->vertex[1];
        QVector3D v2 = i->he->vertex[2];
        QVector3D a = v1 - v0;
        QVector3D b = v2 - v0;
        i->normal = QVector3D::normal(a, b);
	}
}

void HalfEdgeStructure::calculatePerVertexNormals(const float threshold)
{
    const int size(static_cast<int>(m_halfEdges.size()));

	for(int i = 0; i < size; ++i) 
	{
        //TODO
        QVector3D thisNormal = m_halfEdges[i].face->normal;
        HalfEdge *opp = m_halfEdges[i].opp;
//        Face **oppFace = &opp->face;
    }
}
