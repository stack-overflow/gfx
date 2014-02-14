#pragma once
#include "GraphicsSystem.h"
#include "EventProcessor.h"
#include "Object.h"
#include "Scene.h"
#include "Utils.h"

class Game :
    public CommandReceiver
{
public:
    Game()
    {
        m_Graphics  = NULL;
        HandleEvent = NULL;
    }

    Game(GraphicsSystem *_graphics)
    {
        Initialize(_graphics);
    }

    ~Game()
    {
        if (m_Graphics)
        {
            m_Graphics->SceneShutdownBuffers(m_scene);
            delete m_scene;
            m_scene = NULL;

            m_Graphics->Shutdown();
            delete m_Graphics;
            m_Graphics = NULL;
        }
    }

    bool Initialize(GraphicsSystem *_graphics)
    {
        m_Graphics  = _graphics;
        HandleEvent = (CommandReceiver::CommandHandlerFunction)&Game::CommandHandler;

        m_scene = new Scene();

        Model *model = new Model(
            "sphere.xobj",
            new Texture2D("Textures/bricks.bmp", GL_TEXTURE0),
            new Texture2D("Textures/normal_map.bmp", GL_TEXTURE1));

        Object *object = new Object(model);

        m_scene->m_objects.push_back(object);

        m_Graphics->SceneInitializeBuffers(m_scene);
        m_Graphics->ObjectInitializeBuffers(m_scene->m_skybox->m_object);

        return true;
    }

    __forceinline
    bool FrameGame() const
    {
        m_scene->m_Camera->Move(m_cameraVelocity);

        return m_Graphics->Frame(m_scene);
    }

private:
    void CommandHandler(const Event::CommandType _cmd)
    {
        switch (_cmd)
        {
        case Event::GM_CMD_MOVE_FORWARD:
            __DEBUG_OUT("HANDLE: GM_CMD_MOVE_FORWARD");
            m_cameraVelocity.z += 0.2f;
            break;
        case Event::GM_CMD_MOVE_FORWARD_STOP:
            __DEBUG_OUT("HANDLE: GM_CMD_MOVE_FORWARD_STOP");
            m_cameraVelocity.z -= 0.2f;
            break;
        case Event::GM_CMD_MOVE_BACK:
            __DEBUG_OUT("HANDLE: GM_CMD_MOVE_BACK");
            m_cameraVelocity.z -= 0.2f;
            break;
        case Event::GM_CMD_MOVE_BACK_STOP:
            __DEBUG_OUT("HANDLE: GM_CMD_MOVE_BACK_STOP");
            m_cameraVelocity.z += 0.2f;
            break;
        case Event::GM_CMD_MOVE_RIGHT:
            __DEBUG_OUT("HANDLE: GM_CMD_MOVE_RIGHT");
            m_cameraVelocity.x += 0.2f;
            break;
        case Event::GM_CMD_MOVE_RIGHT_STOP:
            __DEBUG_OUT("HANDLE: GM_CMD_MOVE_RIGHT_STOP");
            m_cameraVelocity.x -= 0.2f;
            break;
        case Event::GM_CMD_MOVE_LEFT:
            __DEBUG_OUT("HANDLE: GM_CMD_MOVE_LEFT");
            m_cameraVelocity.x -= 0.2f;
            break;
        case Event::GM_CMD_MOVE_LEFT_STOP:
            __DEBUG_OUT("HANDLE: GM_CMD_MOVE_LEFT_STOP");
            m_cameraVelocity.x += 0.2f;
            break;
        default:
            break;
        }
    }

private:
    GraphicsSystem       *m_Graphics;
    Scene                *m_scene;
    GMath::Vector3f      m_cameraVelocity;
};