#pragma once
#include <Windows.h>
#include "Event.h"

class InputSystem
{
public:
	InputSystem();
	InputSystem(const InputSystem&);
	~InputSystem();

	void Initialize();

    __forceinline
	Event::CommandType KeyPress(unsigned int _key, bool _pressed)
    {
        return (m_keys[_key] != _pressed) ? ProcessKey(_key, _pressed) : Event::GM_CMD_NO_COMMAND;
    }

private:
    __forceinline
    Event::CommandType ProcessKey (unsigned int _key, bool _pressed)
    {
        m_keys[_key] = _pressed;

        switch (_key)
        {
        case 'W':
            return (_pressed) ? Event::GM_CMD_MOVE_FORWARD : Event::GM_CMD_MOVE_FORWARD_STOP;
        case 'S':
            return (_pressed) ? Event::GM_CMD_MOVE_BACK    : Event::GM_CMD_MOVE_BACK_STOP;
        case 'A':
            return (_pressed) ? Event::GM_CMD_MOVE_LEFT    : Event::GM_CMD_MOVE_LEFT_STOP;
        case 'D':
            return (_pressed) ? Event::GM_CMD_MOVE_RIGHT   : Event::GM_CMD_MOVE_RIGHT_STOP;
        case VK_ESCAPE:
            if (!_pressed) return Event::GM_CMD_EXIT;
        default:
            return Event::GM_CMD_NO_COMMAND;
        }
    }

private:
	bool m_keys[256];

};
