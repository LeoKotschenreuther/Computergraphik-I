
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
            m_halfEdges[j].normal = drawable.normals()[l];
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
        QVector3D *v0 = i->he->vertex;
        QVector3D *v1 = i->he->next->vertex;
        QVector3D *v2 = i->he->prev->vertex;
        QVector3D a;
        a.setX(v1->x() - v0->x());
        a.setY(v1->y() - v0->y());
        a.setZ(v1->z() - v0->z());
        QVector3D b;
        b.setX(v2->x() - v0->x());
        b.setY(v2->y() - v0->y());
        b.setZ(v2->z() - v0->z());
        i->normal = QVector3D::normal(a, b);
    }
}

void HalfEdgeStructure::calculatePerVertexNormals(const float threshold)
{
    const int size(static_cast<int>(m_halfEdges.size()));
    qDebug() << threshold;

    for(int i = 0; i < size; ++i)
    {
        //TODO
        int nFaces = 0;
        QVector3D thisNormal = m_halfEdges[i].face->normal;
        HalfEdge *thisHalfEdge = &m_halfEdges[i];
        HalfEdge * he = NULL;
        if(m_halfEdges[i].opp){
             QVector3D *oppNormal = &m_halfEdges[i].opp->face->normal;

            HalfEdge *temp = m_halfEdges[i].opp->next;
            QVector3D avgNormal = thisNormal;
            while (temp != thisHalfEdge  && he == NULL) {
//                qDebug() << "first While";
                float angle = _acosd(QVector3D::dotProduct(thisNormal, temp->face->normal));

                if(angle > threshold){
                    // hard edge
                    m_halfEdges[i].normal = thisNormal;
                }
                else{
                    // soft edge
//                    m_halfEdges[i].normal.setX(0.5f * (thisNormal.x()+oppNormal->x()));
//                    m_halfEdges[i].normal.setY(0.5f * (thisNormal.y()+oppNormal->y()));
//                    m_halfEdges[i].normal.setZ(0.5f * (thisNormal.z()+oppNormal->z()));

                    avgNormal = (avgNormal * (nFaces++) + temp->face->normal) / nFaces;
                }
                if (temp -> opp != NULL) {
                    temp = temp->opp->next;
                } else {
                    he = temp;
                }

            }
            temp = m_halfEdges[i].prev->opp;
            while (temp != NULL && he != NULL && temp != he) {
//                qDebug() << "second While";
                float angle = _acosd(QVector3D::dotProduct(thisNormal, temp->face->normal));

                if(angle > threshold){
                    // hard edge
                    m_halfEdges[i].normal = thisNormal;
                }
                else{
                    // soft edge
//                    m_halfEdges[i].normal.setX(0.5f * (thisNormal.x()+oppNormal->x()));
//                    m_halfEdges[i].normal.setY(0.5f * (thisNormal.y()+oppNormal->y()));
//                    m_halfEdges[i].normal.setZ(0.5f * (thisNormal.z()+oppNormal->z()));

                    avgNormal = (avgNormal * (nFaces++) + temp->face->normal) / nFaces;
                }
                if (temp -> prev -> opp != NULL) {
                    temp = temp->prev->opp;
                } else {
                    break;
                    qDebug () << "nicht beide Richtunge geschafft";
                }
            }

//            qDebug () << "temp == he";


            m_halfEdges[i].normal = avgNormal;

//            qDebug() << angle << threshold;
//            if(angle > threshold){
//                // hard edge
//                m_halfEdges[i].normal = thisNormal;
//            }
//            else{
//                // soft edge
//                m_halfEdges[i].normal.setX(0.5f * (thisNormal.x()+oppNormal->x()));
//                m_halfEdges[i].normal.setY(0.5f * (thisNormal.y()+oppNormal->y()));
//                m_halfEdges[i].normal.setZ(0.5f * (thisNormal.z()+oppNormal->z()));
//            }
        }
        else{
            // I don't know why, but sometimes there exists no opposite haldEdge!!!
            m_halfEdges[i].normal = thisNormal;
        }
    }
}
