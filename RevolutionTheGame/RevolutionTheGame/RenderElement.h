#pragma once
#include "OpenGLSystem.h"
#include "MathUtils.h"
#include "Shader.h"
#include "Object.h"

class RenderElement
{
public:
	RenderElement();
    RenderElement(
        Object *_object,
        GpuProgram *_defaultProgram,
        GMath::Vector3f _scale,
        GMath::Vector3f _position)  
        :
        m_object(_object),
        m_defaultProgram(_defaultProgram),
        m_scale(_scale)
    {}

public:
    Object          *m_object;
    GpuProgram      *m_defaultProgram;

    GMath::Vector3f m_scale;
};
