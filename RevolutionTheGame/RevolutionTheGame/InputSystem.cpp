#include "InputSystem.h"

InputSystem::InputSystem()
{
	Initialize();
}

InputSystem::InputSystem(const InputSystem& _input)
{
    ::memcpy(this->m_keys, _input.m_keys, 256 * sizeof(bool));
}

InputSystem::~InputSystem()
{
}

void InputSystem::Initialize()
{
    for (int i = 0; i < 256; ++i)
    {
        m_keys[i] = false;
    }

	return;
}
