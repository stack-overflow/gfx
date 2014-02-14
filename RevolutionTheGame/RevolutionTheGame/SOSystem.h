#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "OpenGLSystem.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"
#include "Event.h"
#include "Game.h"

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

class SOSystem
{
public:
	SOSystem();
	SOSystem(const SOSystem&);
	~SOSystem();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	bool InitializeWindows(OpenGLSystem*, int&, int&);
	void ShutdownWindows();

	__forceinline
	bool PumpWindowsMessages() const
	{
		static MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
	}

private:
	LPCSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	OpenGLSystem *m_OpenGL;
	InputSystem *m_Input;
    Game *m_Game;
	EventQueue m_eventQueue;
};

static SOSystem *ApplicationHandle = NULL;
