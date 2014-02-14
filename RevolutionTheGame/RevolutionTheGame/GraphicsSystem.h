#pragma once
#include "OpenGLSystem.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Scene.h"
#include "Event.h"
#include "Texture.h"
//#include <windows.h>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsSystem
{
public:
	GraphicsSystem(OpenGLSystem *);
	GraphicsSystem(const GraphicsSystem&);
	~GraphicsSystem();

	bool Initialize(OpenGLSystem *);
	void Shutdown();

	bool Frame(Scene *_scene);

    OpenGLSystem *GetGraphicsApiContext() const;

    TextureManager *GetTextureManager()
    {
        return &m_textureManager;
    }

    bool ObjectInitializeBuffers(Object *object) const;
    void ObjectShutdownBuffer(Object *object) const;

    bool SceneInitializeBuffers(Scene *scene) const;
    void SceneShutdownBuffers(Scene *_scene) const;

private:
	bool RenderSceneObjects(Scene *_scene);
    bool RenderStaticEnv(Scene *_scene);

    void LoadTexture();

private:
    OpenGLSystem *m_OpenGL;
	TextureManager m_textureManager;
};