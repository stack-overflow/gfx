#pragma once
#include <vector>
#include <memory>

#include "OpenGLSystem.h"
#include "Texture.h"
#include "MathUtils.h"
#include "Shader.h"

struct Model
{
public:
	typedef std::vector<GMath::Vertex> VerticesType;
public:
	Model();
	Model(const VerticesType& ver);
	Model(const VerticesType& ver, Texture *_tex);
	Model(const char *_verFilename, Texture *_tex);
	Model(const char *_verFilename, Texture *_tex, Texture2D *_normalMap);
	~Model();
    
    __forceinline
    Texture *const GetTexture() const
    {
	    return m_texture.get();
    }
    __forceinline
    Texture2D *const GetNormalMap() const
    {
	    return m_normalMap.get();
    }
    __forceinline
    unsigned int GetVertexArrayId() const
    {
        return m_vertexArrayId;
    }
    __forceinline
    unsigned int GetVertexBufferId() const
    {
        return m_vertexBufferId;
    }
    __forceinline
    int GetMeshSize() const
    {
        return m_meshSize;
    }
    __forceinline
    GMath::Vertex *GetVertices()
    {
        return vertices.data();
    }
    __forceinline
    void SetVertexArrayId(unsigned int _vao)
    {
        m_vertexArrayId = _vao;
    }
    __forceinline
    void SetVertexBufferId(unsigned int _buffer)
    {
        m_vertexBufferId = _buffer;
    }
private:
	void LoadVerticesFromFile(const char *_filename);
	void InitializeBuffers();
	void ShutdownBuffers();

private:
    OpenGLSystem *m_OpenGL;
	std::vector<GMath::Vertex> vertices;
	unsigned int               m_vertexArrayId;
	unsigned int               m_vertexBufferId;
	std::unique_ptr<Texture>   m_texture;
	std::unique_ptr<Texture2D> m_normalMap;

	void ComputeTangentsBitangents();

	int m_meshSize;
};
