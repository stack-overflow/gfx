#pragma once
#include <vector>
#include <functional>
#include <algorithm>

#include "Model.h"
#include "Light.h"
#include "RenderElement.h"
#include "Object.h"

class Skybox :
    public RenderElement
{
public:
    Skybox(GMath::Vector3f _scale) :
        RenderElement()
    {
        m_scale = _scale;
        Initialize();
    }

    void Initialize()
    {
        TextureCubeMap *texSky = new TextureCubeMap();
        texSky->SetTextureUnit(GL_TEXTURE2);
        texSky->LoadTextureFaceFromBMPFile("Textures/skybox1.bmp", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        texSky->LoadTextureFaceFromBMPFile("Textures/skybox2.bmp", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
        texSky->LoadTextureFaceFromBMPFile("Textures/skybox3.bmp", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
        texSky->LoadTextureFaceFromBMPFile("Textures/skybox4.bmp", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
        texSky->LoadTextureFaceFromBMPFile("Textures/skybox5.bmp", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
        texSky->LoadTextureFaceFromBMPFile("Textures/skybox6.bmp", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

        Model *skyboxModel = new Model(
            "box.xobj",
            texSky);

        m_object         = new Object(skyboxModel);
        m_defaultProgram = new GpuProgram("skybox.vp", "skybox.fp");
    }
};

#if 0
class ReflectionSkybox :
    public RenderElement
{
    ReflectionSkybox(OpenGLSystem *_OpenGL) :
        RenderElement()
    {
        Initialize();
    }

    void Initialize()
    {
        TextureCubeMap *texSky = new TextureCubeMap();
        texSky->SetTextureUnit(GL_TEXTURE2);
        texSky->LoadTextureFaceFromBMPFile("Textures/skybox1.bmp", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        texSky->LoadTextureFaceFromBMPFile("Textures/skybox2.bmp", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
        texSky->LoadTextureFaceFromBMPFile("Textures/skybox3.bmp", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
        texSky->LoadTextureFaceFromBMPFile("Textures/skybox4.bmp", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
        texSky->LoadTextureFaceFromBMPFile("Textures/skybox5.bmp", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
        texSky->LoadTextureFaceFromBMPFile("Textures/skybox6.bmp", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

        m_Shader_reflect = new GpuProgram(
            "reflect.vp",
            "reflect.fp");

        m_Model_reflect = new Model(
            "sphere.xobj",
            texSky);
    }

    void Render(GMath::Matrix4f& viewMatrix, GMath::Matrix4f& projectionMatrix)
    {
        // Reflected sphere rendering

        GMath::Matrix4f modelMatrix;
        m_OpenGL->GetWorldMatrix(modelMatrix);

        m_OpenGL->MatrixScale(modelMatrix, GMath::Vector3f(2, 2, 2));

        m_Shader_reflect->UseProgram();
        m_Model_reflect->GetTexture()->Use();
        m_Shader_reflect->Uniform1i("colorMap", m_Model_reflect->GetTexture()->GetSampler());
        m_Shader_reflect->UniformMatrix4fv("modelMatrix", 1, false, (float*)(&modelMatrix));
        m_Shader_reflect->UniformMatrix4fv("viewMatrix", 1, false, (float*)(&viewMatrix));
        m_Shader_reflect->UniformMatrix4fv("projectionMatrix", 1, false, (float*)(&projectionMatrix));
    }

private:
    OpenGLSystem *m_OpenGL;
    Model        *m_Model_reflect;
    GpuProgram   *m_Shader_reflect;
};
#endif

class Scene
{
public:
    Scene()
    {
        Initialize();
    }

    ~Scene()
    {
        std::for_each(
            m_objects.begin(),
            m_objects.end(),
            std::default_delete<Object>());

        delete m_ambientLight;
        delete m_diffuseLight;
        delete m_defaultProgram;
        delete m_skybox;
    }

    friend class GraphicsSystem;
    friend class Game;

    bool Initialize()
    {
        m_Camera = new Camera();

		m_defaultProgram = new GpuProgram();

		m_defaultProgram->LoadVertexShaderFromFile("standard_color.vp");
		m_defaultProgram->LoadFragmentShaderFromFile("standard_color.fp");
		m_defaultProgram->BuildProgram();

		// Also possible:
		//m_defaultProgram = new GpuProgram(
		//    "standard_color.vp",
		//    "standard_color.fp");


        if (!m_defaultProgram)
        {
            return false;
        }

        m_diffuseLight = new PositionLight(GMath::Vector3f(10.0f, 10.0f, 10.0f), GMath::Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
        m_ambientLight = new Light(GMath::Vector4f(0.4f, 0.4f, 0.4f, 1.0f));

	    m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

        m_skybox = new Skybox(GMath::Vector3f(12.0f, 12.0f, 12.0f));

        return true;
    }

private:
    //static int SceneNum;
    int m_sceneId;

    GpuProgram           *m_defaultProgram;
    PositionLight        *m_diffuseLight;
    Light                *m_ambientLight;

    Camera               *m_Camera;
    Skybox               *m_skybox;

    std::vector <Object *> m_objects;
};

//int Scene::SceneNum = 0;