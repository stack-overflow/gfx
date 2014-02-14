#pragma once
#include <cassert>
#include "Utils.h"

struct Event
{
public:
	enum SYS_EventType
	{
		EV_KEY,
		EV_MOUSE,
        EV_EXIT,
		EV_NO_EVENT,
	};

    enum CommandType
    {
        GM_CMD_MOVE_FORWARD = 1024,
        GM_CMD_MOVE_FORWARD_STOP,
        GM_CMD_MOVE_BACK,
        GM_CMD_MOVE_BACK_STOP,
        GM_CMD_MOVE_RIGHT,
        GM_CMD_MOVE_RIGHT_STOP,
        GM_CMD_MOVE_LEFT,
        GM_CMD_MOVE_LEFT_STOP,
        GM_CMD_TURN_RIGHT,
        GM_CMD_TURN_LEFT,
        GM_CMD_JUMP,
        GM_CMD_NO_COMMAND,
        GM_CMD_EXIT,
        UI_CMD_CLICK,
        UI_CMD_NO_COMMAND
    };

public:
	Event() :
		type(EV_NO_EVENT)
	{}

	Event(SYS_EventType _type) :
		type(_type)
	{}

	Event(SYS_EventType _type, int _arg) :
		type(_type),
		argA(_arg)
	{}

	Event(SYS_EventType _type, int _argA, int _argB) :
		type(_type),
		argA(_argA),
		argB(_argB)
	{}

	SYS_EventType type;
	int           argA;
	int           argB;
};

template <int MAX_NUM_EVENTS>
class PrototypeEventQueue
{
public:
	static Event NoEvent;

public:
	PrototypeEventQueue() :
		m_eventNum(0)
	{
		m_currentEvent = (char *)m_eventQueue;
	}

    	__forceinline
	void AddEvent(Event::SYS_EventType _eventType)
	{
		assert(m_eventNum < MAX_NUM_EVENTS);
		m_eventQueue[m_eventNum++] = Event(_eventType);
	}

	__forceinline
	void AddEvent(Event::SYS_EventType _eventType, int _arg)
	{
		assert(m_eventNum < MAX_NUM_EVENTS);
		m_eventQueue[m_eventNum++] = Event(_eventType, _arg);
	}

	__forceinline
	void AddEvent(Event::SYS_EventType _eventType, int _argA, int _argB)
	{
		assert(m_eventNum < MAX_NUM_EVENTS);

		m_eventQueue[m_eventNum++] = Event(_eventType, _argA, _argB);
	}

	__forceinline
	Event& GetEvent()
	{
		if (m_currentEvent < (char *)(&m_eventQueue[m_eventNum]))
		{
			Event *toReturn = (Event *)m_currentEvent;
			m_currentEvent += sizeof(Event);
			return *toReturn;
		}
		else
		{
			m_eventNum = 0;
			m_currentEvent = (char *)m_eventQueue;
			return NoEvent;
		}

	}

	__forceinline
	void Clear()
	{
		m_eventNum = 0;
		m_currentEvent = (char *) m_eventQueue;
	}

	__forceinline
	int GetMaxNumEvents() const
	{
		return MAX_NUM_EVENTS;
	}

private:
	int   m_eventNum;
	Event m_eventQueue[MAX_NUM_EVENTS];
	char *m_currentEvent;
};

typedef PrototypeEventQueue<256> EventQueue;

Event EventQueue::NoEvent = Event(Event::EV_NO_EVENT, 0);
