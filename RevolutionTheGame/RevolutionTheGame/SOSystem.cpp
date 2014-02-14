#include "SOSystem.h"
#include "Model.h"

SOSystem::SOSystem()
{
	m_OpenGL   = NULL;
	m_Input    = NULL;
}

SOSystem::SOSystem(const SOSystem& so)
{
}

SOSystem::~SOSystem()
{
	Shutdown();
}

bool SOSystem::Initialize()
{
	int screenWidth = 0, screenHeight = 0;
	bool result;

	m_OpenGL = new OpenGLSystem();
	if (!m_OpenGL)
	{
		return false;
	}

	result = InitializeWindows(m_OpenGL, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	m_Input = new InputSystem();
	if (!m_Input)
	{
		return false;
	}
	
	m_Input->Initialize();

	GraphicsSystem *graphics = new GraphicsSystem(m_OpenGL);
	if (!graphics)
	{
		return false;
	}

    m_Game = new Game();

    if (!m_Game->Initialize(graphics))
    {
        return false;
    }

	return true;
}

void SOSystem::Shutdown()
{
	if (m_Input)
	{
		delete m_Input;
		m_Input = NULL;
	}

	if (m_OpenGL)
	{
		delete m_OpenGL;
		m_OpenGL = NULL;
	}
}

void SOSystem::Run()
{
	bool running = true;
	EventProcessor<InputSystem> processEvents(&m_eventQueue);

    processEvents.RegisterGameCommandReceiver(m_Game);

	// Main game loop.
	while (running)
	{
		running = PumpWindowsMessages();
		running = (running) ? processEvents() : false;
		running = (running) ? Frame()         : false;
	}
}

// Single frame actions, after processing events.
bool SOSystem::Frame()
{
	return m_Game->FrameGame();
}

bool SOSystem::InitializeWindows(OpenGLSystem* OpenGL, int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	ApplicationHandle = this;

	m_hinstance = GetModuleHandle(NULL);

	m_applicationName = "Revolution";

	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	// Temporary OpenGL window
	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		m_applicationName,
		m_applicationName,
		WS_POPUP,
		0, 0,
		640, 480,
		NULL,
		NULL,
		m_hinstance,
		NULL);

	if (m_hwnd == NULL)
	{
		return false;
	}

	ShowWindow(m_hwnd, SW_HIDE);

	// OpenGL initialization
    if (!OpenGL->InitializeExtensions(m_hwnd))
    {
        return false;
    }

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Regular window
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = static_cast<DWORD>(screenWidth);
		dmScreenSettings.dmPelsHeight = static_cast<DWORD>(screenHeight);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		screenWidth  = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		m_applicationName,
		m_applicationName,
		WS_POPUP,
		posX, posY,
		screenWidth,
		screenHeight,
		NULL,
		NULL,
		m_hinstance,
		NULL);

	if (m_hwnd == NULL)
	{
		return false;
	}

    if (!OpenGL->InitializeOpenGL(m_hwnd, screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, VSYNC_ENABLED))
    {
        return false;
    }

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	ShowCursor(FALSE);

	return true;
}

void SOSystem::ShutdownWindows()
{
	ShowCursor(TRUE);
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK SOSystem::MessageHandler(
	HWND hwnd,
	UINT umsg,
	WPARAM wparam,
	LPARAM lparam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
		{
			m_eventQueue.AddEvent(Event::EV_KEY, static_cast<unsigned int>(wparam), TRUE);
			return 0;
		}

	case WM_KEYUP:
		{
			m_eventQueue.AddEvent(Event::EV_KEY, static_cast<unsigned int>(wparam), FALSE);
			return 0;
		}

    case WM_QUIT:
        {
            m_eventQueue.AddEvent(Event::EV_EXIT);
            return 0;
        }
	default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT umessage,
	WPARAM wparam,
	LPARAM lparam)
{
	switch (umessage)
	{
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
