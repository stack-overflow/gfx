#pragma  once
#include "Utils.h"
#include "Event.h"
#include "InputSystem.h"

class CommandReceiver
{
public:
    __forceinline
    void ProcessCommand(const Event::CommandType _event)
    {
        (this->*HandleEvent)(_event);
    }

protected:
    typedef void (CommandReceiver::*CommandHandlerFunction)(const Event::CommandType _cmd);
    CommandHandlerFunction HandleEvent;
};

template<class InputSystemClass>
class EventProcessor
{
public:
    EventProcessor(EventQueue *_eventQueue) :
        m_internalEventQueue(_eventQueue),
        m_internalInputSystem(new InputSystemClass())
    {}
public:
    __forceinline
    bool operator ()() const
    {
        Event ev;
        while ( (ev = m_internalEventQueue->GetEvent()).type != Event::EV_NO_EVENT )
        {
            switch (ev.type)
            {
            case Event::EV_KEY:
                {
                    __DEBUG_OUT_NO_FLUSH("Key: ");
                    __DEBUG_OUT_NO_FLUSH(ev.argA);
                    __DEBUG_OUT_NO_FLUSH(ev.argB ? " pressed" : " released");
                    __DEBUG_OUT_DO_FLUSH;

                    Event::CommandType cmd = m_internalInputSystem->KeyPress(ev.argA, ev.argB != 0);
                    if (cmd != Event::GM_CMD_NO_COMMAND)
                    {
                        if (cmd == Event::GM_CMD_EXIT)
                        {
                            return false;
                        }
                        m_gameEventReceiver->ProcessCommand(cmd);
                    }
                }
                break;
            case Event::EV_MOUSE:
                break;
            case Event::EV_EXIT:
                return false;
            default:
                break;
            }
        }
        return true;
    }
    __forceinline
    void ClearQueue() const
    {
        m_internalEventQueue->Clear();
    }
    __forceinline
    void BindQueue(EventQueue *_eventQueue)
    {
        m_internalEventQueue = _eventQueue;
    }
    __forceinline
    EventQueue *GetCurrentQueue() const
    {
        return m_internalEventQueue;
    }
    __forceinline
    void RegisterGameCommandReceiver(CommandReceiver *_gameEventReceiver)
    {
        __DEBUG_OUT("Registering Game");
        m_gameEventReceiver = _gameEventReceiver;
    }
    __forceinline
    void RegisterUICommandReceiver(CommandReceiver *_uiEventReceiver)
    {
        m_uiEventReceiver = _uiEventReceiver;
    }
private:
    EventProcessor(const EventProcessor&);
private:
    EventQueue       *m_internalEventQueue;
    InputSystemClass *m_internalInputSystem;

    CommandReceiver  *m_gameEventReceiver;
    CommandReceiver  *m_uiEventReceiver;
};
