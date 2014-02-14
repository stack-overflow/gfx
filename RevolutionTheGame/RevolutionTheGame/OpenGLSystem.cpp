#include "OpenGLSystem.h"

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

PFNGLATTACHSHADERPROC glAttachShader;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM4FVPROC glUniform4fv;

OpenGLSystem::OpenGLSystem()
{
    m_deviceContext = 0;
    m_renderingContext = 0;
}

OpenGLSystem::OpenGLSystem(const OpenGLSystem&)
{
}

OpenGLSystem::~OpenGLSystem()
{
    Shutdown(m_hwnd);
}

bool OpenGLSystem::InitializeExtensions(HWND hwnd)
{
    HDC deviceContext;
    PIXELFORMATDESCRIPTOR pixelFormat;
    int error;
    HGLRC renderContext;
    bool result;

    deviceContext = GetDC(hwnd);
    if (!deviceContext)
    {
        return false;
    }
    
    error = SetPixelFormat(deviceContext, 1, &pixelFormat);
    if (error != TRUE)
    {
        return false;
    }

    renderContext = wglCreateContext(deviceContext);
    if (!renderContext)
    {
        return false;
    }

    error = wglMakeCurrent(deviceContext, renderContext);
    if (error != TRUE)
    {
        return false;
    }

    result = LoadExtensionList();
    if (!result)
    {
        return false;
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(renderContext);
    renderContext = NULL;
    ReleaseDC(hwnd, deviceContext);
    deviceContext = NULL;

    return true;
}

bool OpenGLSystem::InitializeOpenGL(HWND hwnd, int screenWidth, int screenHeight, float screenDepth, float screenNear, bool vsync)
{
    int attribListInit[19];
    int pixelFormat[1];
    unsigned int formatCount;
    int result;
    PIXELFORMATDESCRIPTOR pixelFormatDesc;
    int attribListOGL[5];
    float fieldOfView, screenAspect;
    char *vendorString, *rendererString;

    m_deviceContext = GetDC(hwnd);
    if (!m_deviceContext)
    {
        return false;
    }

    m_hwnd = hwnd;

    attribListInit[0] = WGL_SUPPORT_OPENGL_ARB;
    attribListInit[1] = TRUE;

    attribListInit[2] = WGL_DRAW_TO_WINDOW_ARB;
    attribListInit[3] = TRUE;
    
    attribListInit[4] = WGL_ACCELERATION_ARB;
    attribListInit[5] = WGL_FULL_ACCELERATION_ARB;
    
    attribListInit[6] = WGL_COLOR_BITS_ARB;
    attribListInit[7] = 24;
    
    attribListInit[8] = WGL_DEPTH_BITS_ARB;
    attribListInit[9] = 24;

    attribListInit[10] = WGL_DOUBLE_BUFFER_ARB;
    attribListInit[11] = TRUE;

    attribListInit[12] = WGL_SWAP_METHOD_ARB;
    attribListInit[13] = WGL_SWAP_EXCHANGE_ARB;

    attribListInit[14] = WGL_PIXEL_TYPE_ARB;
    attribListInit[15] = WGL_TYPE_RGBA_ARB;

    attribListInit[16] = WGL_STENCIL_BITS_ARB;
    attribListInit[17] = 8;

	//attribListInit[18] = WGL_CONTEXT_PROFILE_MASK_ARB;
	//attribListInit[19] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;

    attribListInit[18] = 0;

    result = wglChoosePixelFormatARB(m_deviceContext, attribListInit, NULL, 1, pixelFormat, &formatCount);
    if (result != TRUE)
    {
        return false;
    }

    result = SetPixelFormat(m_deviceContext, pixelFormat[0], &pixelFormatDesc);
    if (result != 1)
    {
        return false;
    }

    attribListOGL[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
    attribListOGL[1] = 4;
    attribListOGL[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
    attribListOGL[3] = 0;

    attribListOGL[4] = 0;

	// Try init OpenGL 4.0
    m_renderingContext = wglCreateContextAttribsARB(m_deviceContext, m_renderingContext, attribListOGL);
    if (!m_renderingContext)
    {
		// If failed try OpenGL 3.3
		attribListOGL[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
		attribListOGL[1] = 3;
		attribListOGL[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
		attribListOGL[3] = 3;

		m_renderingContext = wglCreateContextAttribsARB(m_deviceContext, m_renderingContext, attribListOGL);

		if (!m_renderingContext)
		{
			return false;
		}
    }

    result = wglMakeCurrent(m_deviceContext, m_renderingContext);
    if (result != 1)
    {
        return false;
    }

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //
    fieldOfView =  PI / 4.0f;
    screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	m_projectionMatrix = GMath::BuildPerspectiveFovLHMatrix(fieldOfView, screenAspect, screenNear, screenDepth);
	m_worldMatrix = GMath::Matrix4f::Identity();

    vendorString   = (char *)glGetString(GL_VENDOR);
    rendererString = (char *)glGetString(GL_RENDERER);

    strcpy_s(m_videoCardDescription, vendorString);
    strcpy_s(m_videoCardDescription, "-");
    strcpy_s(m_videoCardDescription, rendererString);

    if (vsync)
    {
        result = wglSwapIntervalEXT(1);
    }
    else
    {
        result = wglSwapIntervalEXT(0);
    }

    if (result != TRUE)
    {
        return false;
    }

    return true;
}

void OpenGLSystem::Shutdown(HWND hwnd)
{
    if (m_renderingContext)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_renderingContext);
        m_renderingContext = NULL;
    }

    if (m_deviceContext)
    {
        ReleaseDC(hwnd, m_deviceContext);
        m_deviceContext = NULL;
    }
}

void OpenGLSystem::BeginScene(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLSystem::EndScene()
{
    SwapBuffers(m_deviceContext);
}

bool OpenGLSystem::LoadExtensionList()
{
    // Load the OpenGL extensions that this application will be using.
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    if(!wglChoosePixelFormatARB)
    {
        return false;
    }

    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if(!wglCreateContextAttribsARB)
    {
        return false;
    }

    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if(!wglSwapIntervalEXT)
    {
        return false;
    }

    glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
    if(!glAttachShader)
    {
        return false;
    }

    glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    if(!glBindBuffer)
    {
        return false;
    }

    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
    if(!glBindVertexArray)
    {
        return false;
    }

    glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    if(!glBufferData)
    {
        return false;
    }

    glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
    if(!glCompileShader)
    {
        return false;
    }

    glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
    if(!glCreateProgram)
    {
        return false;
    }

    glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
    if(!glCreateShader)
    {
        return false;
    }

    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
    if(!glDeleteBuffers)
    {
        return false;
    }

    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
    if(!glDeleteProgram)
    {
        return false;
    }

    glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
    if(!glDeleteShader)
    {
        return false;
    }

    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
    if(!glDeleteVertexArrays)
    {
        return false;
    }

    glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
    if(!glDetachShader)
    {
        return false;
    }

    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
    if(!glEnableVertexAttribArray)
    {
        return false;
    }

    glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    if(!glGenBuffers)
    {
        return false;
    }

    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
    if(!glGenVertexArrays)
    {
        return false;
    }

    glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
    if(!glGetAttribLocation)
    {
        return false;
    }

    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
    if(!glGetProgramInfoLog)
    {
        return false;
    }

    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
    if(!glGetProgramiv)
    {
        return false;
    }

    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
    if(!glGetShaderInfoLog)
    {
        return false;
    }

    glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
    if(!glGetShaderiv)
    {
        return false;
    }

    glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
    if(!glLinkProgram)
    {
        return false;
    }

    glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
    if(!glShaderSource)
    {
        return false;
    }

    glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
    if(!glUseProgram)
    {
        return false;
    }

    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
    if(!glVertexAttribPointer)
    {
        return false;
    }

    glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
    if(!glBindAttribLocation)
    {
        return false;
    }

    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
    if(!glGetUniformLocation)
    {
        return false;
    }

    glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");
    if (!glUniformMatrix3fv)
    {
        return false;
    }

    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
    if(!glUniformMatrix4fv)
    {
        return false;
    }

    glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
    if(!glActiveTexture)
    {
        return false;
    }

    glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
    if(!glUniform1i)
    {
        return false;
    }

    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
    if(!glGenerateMipmap)
    {
        return false;
    }

    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
    if(!glDisableVertexAttribArray)
    {
        return false;
    }

    glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
    if(!glUniform3fv)
    {
        return false;
    }

    glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
    if(!glUniform4fv)
    {
        return false;
    }

    return true;
}

void OpenGLSystem::GetWorldMatrix(GMath::Matrix4f& matrix)
{
	matrix[0] = m_worldMatrix[0];
	matrix[1] = m_worldMatrix[1];
	matrix[2] = m_worldMatrix[2];
	matrix[3] = m_worldMatrix[3];

	matrix[4] = m_worldMatrix[4];
	matrix[5] = m_worldMatrix[5];
	matrix[6] = m_worldMatrix[6];
	matrix[7] = m_worldMatrix[7];

	matrix[8] = m_worldMatrix[8];
	matrix[9] = m_worldMatrix[9];
	matrix[10] = m_worldMatrix[10];
	matrix[11] = m_worldMatrix[11];

	matrix[12] = m_worldMatrix[12];
	matrix[13] = m_worldMatrix[13];
	matrix[14] = m_worldMatrix[14];
	matrix[15] = m_worldMatrix[15];
}

void OpenGLSystem::GetProjectionMatrix(GMath::Matrix4f& matrix)
{
    matrix[0]  = m_projectionMatrix[0];
    matrix[1]  = m_projectionMatrix[1];
    matrix[2]  = m_projectionMatrix[2];
    matrix[3]  = m_projectionMatrix[3];

    matrix[4]  = m_projectionMatrix[4];
    matrix[5]  = m_projectionMatrix[5];
    matrix[6]  = m_projectionMatrix[6];
    matrix[7]  = m_projectionMatrix[7];

    matrix[8]  = m_projectionMatrix[8];
    matrix[9]  = m_projectionMatrix[9];
    matrix[10] = m_projectionMatrix[10];
    matrix[11] = m_projectionMatrix[11];

    matrix[12] = m_projectionMatrix[12];
    matrix[13] = m_projectionMatrix[13];
    matrix[14] = m_projectionMatrix[14];
    matrix[15] = m_projectionMatrix[15];
}

void OpenGLSystem::GetVideoCardInfo(char *cardName)
{
    strcpy_s(cardName, 128, m_videoCardDescription);
}
