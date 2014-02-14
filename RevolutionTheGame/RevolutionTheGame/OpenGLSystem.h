//#pragma once
#ifndef __OPENGL_SYSTEM__
#define __OPENGL_SYSTEM__

#include <Windows.h>
#include <gl\GL.h>
#include "wglext.h"
#include "glext.h"
#include "MathUtils.h"


// Link OpenGL Windows lib
#pragma comment(lib, "opengl32.lib")

#define PI 3.141592653589793238462643383279502884197169399375105820974944592f

class OpenGLSystem
{
public:
	OpenGLSystem();
	OpenGLSystem(const OpenGLSystem&);
	~OpenGLSystem();

    bool InitializeExtensions(HWND);
    bool InitializeOpenGL(HWND, int, int, float, float, bool);
    void Shutdown(HWND);

    void BeginScene(float, float, float, float);
    void EndScene();

    void GetWorldMatrix(GMath::Matrix4f&);
    void GetProjectionMatrix(GMath::Matrix4f&);
    void GetVideoCardInfo(char *);
private:
    bool LoadExtensionList();

private:
    HWND m_hwnd;
    HDC m_deviceContext;
    HGLRC m_renderingContext;

	GMath::Matrix4f m_worldMatrix;
    GMath::Matrix4f m_projectionMatrix;

    char m_videoCardDescription[128];

};

    extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
    extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
    extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

    extern PFNGLATTACHSHADERPROC glAttachShader;
    extern PFNGLBINDBUFFERPROC glBindBuffer;
    extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
    extern PFNGLBUFFERDATAPROC glBufferData;
    extern PFNGLCOMPILESHADERPROC glCompileShader;
    extern PFNGLCREATEPROGRAMPROC glCreateProgram;
    extern PFNGLCREATESHADERPROC glCreateShader;
    extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
    extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
    extern PFNGLDELETESHADERPROC glDeleteShader;
    extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
    extern PFNGLDETACHSHADERPROC glDetachShader;
    extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    extern PFNGLGENBUFFERSPROC glGenBuffers;
    extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
    extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
    extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
    extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
    extern PFNGLGETSHADERIVPROC glGetShaderiv;
    extern PFNGLLINKPROGRAMPROC glLinkProgram;
    extern PFNGLSHADERSOURCEPROC glShaderSource;
    extern PFNGLUSEPROGRAMPROC glUseProgram;
    extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
    extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
    extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
    extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
    extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
    extern PFNGLACTIVETEXTUREPROC glActiveTexture;
    extern PFNGLUNIFORM1IPROC glUniform1i;
    extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
    extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
    extern PFNGLUNIFORM3FVPROC glUniform3fv;
    extern PFNGLUNIFORM4FVPROC glUniform4fv;

#endif //__OPENGL_SYSTEM__