#pragma once

#include "abstractGLexercise.h"

#include <math.h>
#include <string>
#include <vector>
#include <QGLShaderProgram>

class PolygonalDrawable;
class HalfEdgeStructure;

enum ShadingMode
{
    Toon_Shading,
    Phong_Shading,
    NumShadingModes
};

enum MaterialMode
{
    Gold,
    Red_Plastic,
    Jade,
    Chrome,
    NumMaterialModes
};

class Exercise20 : public AbstractGLExercise
{
public:
    Exercise20(QWidget *parent = NULL);
    virtual ~Exercise20();

    virtual void paintGL();
    virtual void initializeGL();
    virtual const bool initialize();

    virtual const QString hints() const;

protected:
	virtual void keyPressEvent(QKeyEvent * keyEvent);

    void draw();
    void setupLight();
    void setupMaterials();
    void loadToonProgram();
    void loadPhongProgram();

	void setupLightUniforms(QGLShaderProgram* prog);
	void setupMaterialUniforms(QGLShaderProgram* prog);

protected:
    ShadingMode         m_shadingMode;
    MaterialMode        m_materialMode;

    MaterialDefinition  *m_materials;
    LightingDefinition  m_lighting;
    PolygonalDrawable   *m_drawable;
    HalfEdgeStructure   *m_he;
	QGLShaderProgram	*m_prog_toon;
	QGLShaderProgram	*m_prog_phong;

};
