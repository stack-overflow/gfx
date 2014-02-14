#include "GraphicsSystem.h"
#include <functional>

GraphicsSystem::GraphicsSystem(OpenGLSystem *OpenGL)
{
    m_OpenGL = NULL;

	Initialize(OpenGL);
}

GraphicsSystem::GraphicsSystem(const GraphicsSystem&)
{
}

GraphicsSystem::~GraphicsSystem()
{
	Shutdown();
}

OpenGLSystem *GraphicsSystem::GetGraphicsApiContext() const
{
    return m_OpenGL;
}

bool GraphicsSystem::Initialize(OpenGLSystem *OpenGL)
{
    m_OpenGL = OpenGL;

	return true;
}



void GraphicsSystem::Shutdown()
{
    m_OpenGL = 0;
}

bool GraphicsSystem::Frame(Scene *_scene)
{
    m_OpenGL->BeginScene(0.0f, 0.0f, 0.0, 1.0f);

    if (!RenderStaticEnv(_scene))
    {
        return false;
    }

    if (!RenderSceneObjects(_scene))
    {
        return false;
    }

    m_OpenGL->EndScene();

	return true;
}

bool GraphicsSystem::ObjectInitializeBuffers(Object *object) const
{
    GLuint vertexArrayId, vertexBufferId;

	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
	glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(
		GL_ARRAY_BUFFER,
        object->GetModel()->GetMeshSize() * sizeof(GMath::Vertex),
		(GLvoid*)(object->GetModel()->GetVertices()),
		GL_STATIC_DRAW);

	// To do.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	// Model coords
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(GMath::Vertex), (GLvoid*)(offsetof(GMath::Vertex, pos)));

	// Normal coords
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(GMath::Vertex), (GLvoid*)(offsetof(GMath::Vertex, nor)));

	// Texture coords
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(GMath::Vertex), (GLvoid*)(offsetof(GMath::Vertex, tex)));

	// Tangents
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(GMath::Vertex), (GLvoid*)(offsetof(GMath::Vertex, tangent)));

	// Biangents
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(GMath::Vertex), (GLvoid*)(offsetof(GMath::Vertex, bitangent)));

    object->GetModel()->SetVertexArrayId(vertexArrayId);
    object->GetModel()->SetVertexBufferId(vertexBufferId);

    return true;
}

bool GraphicsSystem::SceneInitializeBuffers(Scene *_scene) const
{
    for (Object *object : _scene->m_objects)
    {
        ObjectInitializeBuffers(object);
    }

    return true;
}

void GraphicsSystem::ObjectShutdownBuffer(Object *object) const
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

    GLuint vbo = object->GetModel()->GetVertexBufferId();
    GLuint vao = object->GetModel()->GetVertexArrayId();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
}

void GraphicsSystem::SceneShutdownBuffers(Scene *_scene) const
{
    for (Object *object : _scene->m_objects)
    {
        ObjectShutdownBuffer(object);
    }
}

bool GraphicsSystem::RenderStaticEnv(Scene *_scene)
{
	Skybox *skybox = _scene->m_skybox;

	GMath::Matrix4f modelMatrix = GMath::Matrix4f::Scale(skybox->m_scale);
	GMath::Matrix4f viewMatrix;
	GMath::Matrix4f projectionMatrix;

	_scene->m_Camera->GetViewMatrix(viewMatrix);
	m_OpenGL->GetProjectionMatrix(projectionMatrix);

	skybox->m_defaultProgram->UseProgram();
	skybox->m_object->GetModel()->GetTexture()->Use();
	skybox->m_defaultProgram->Uniform1i("colorMap", skybox->m_object->GetModel()->GetTexture()->GetSampler());
	skybox->m_defaultProgram->UniformMatrix4fv("modelMatrix", 1, false, (float*) (&modelMatrix));
	skybox->m_defaultProgram->UniformMatrix4fv("viewMatrix", 1, false, (float*) (&viewMatrix));
	skybox->m_defaultProgram->UniformMatrix4fv("projectionMatrix", 1, false, (float*) (&projectionMatrix));

	glCullFace(GL_FRONT);
	glBindVertexArray(skybox->m_object->GetModel()->GetVertexArrayId());
	glDrawArrays(GL_TRIANGLES, 0, skybox->m_object->GetModel()->GetMeshSize());
	glCullFace(GL_BACK);

	return true;
}

bool GraphicsSystem::RenderSceneObjects(Scene *_scene)
{
	GMath::Matrix4f modelMatrix;
    GMath::Matrix4f viewMatrix;
	GMath::Matrix4f projectionMatrix;

	_scene->m_Camera->Render();
	_scene->m_Camera->GetViewMatrix(viewMatrix);
	
	m_OpenGL->GetWorldMatrix(modelMatrix);
	m_OpenGL->GetProjectionMatrix(projectionMatrix);

	// Scene rendering

    _scene->m_defaultProgram->UseProgram();
    _scene->m_defaultProgram->UniformMatrix4fv("modelMatrix", 1, false, (float*)(&modelMatrix));
    _scene->m_defaultProgram->UniformMatrix4fv("viewMatrix", 1, false, (float*)(&viewMatrix));
    _scene->m_defaultProgram->UniformMatrix4fv("projectionMatrix", 1, false, (float*)(&projectionMatrix));

    _scene->m_defaultProgram->Uniform3fv("lightPos", 1, (float*)&(_scene->m_diffuseLight->GetPosition()));
    _scene->m_defaultProgram->Uniform4fv("lightColor", 1, (float*)&(_scene->m_diffuseLight->GetColor()));
    _scene->m_defaultProgram->Uniform4fv("ambientLightColor", 1, (float*)&(_scene->m_ambientLight->GetColor()));

    for (Object *object : _scene->m_objects)
    {
        object->GetModel()->GetTexture()->Use();
        _scene->m_defaultProgram->Uniform1i("colorMap", object->GetModel()->GetTexture()->GetSampler());
        _scene->m_defaultProgram->Uniform1i("normalMap", object->GetModel()->GetNormalMap()->GetSampler());

        glBindVertexArray(object->GetModel()->GetVertexArrayId());
        glDrawArrays(GL_TRIANGLES, 0, object->GetModel()->GetMeshSize());
    }

	return true;
}
