#pragma once
#include "TextBuffer.h"
#include "OpenGLSystem.h"

class ShaderProgram
{
private:
	GLuint shaderId;
	GLenum shaderType;
	GLint status;
	TextBuffer src;

private:
	void LoadSource() {
		GLint programLength = src.GetLength();
		const GLchar* sourceFileName = static_cast<const GLchar*>(src.GetText());
		glShaderSource(shaderId, 1, &sourceFileName, &programLength);
	}

protected:
	ShaderProgram(GLenum _shaderType) :
        
		shaderType(_shaderType),
		shaderId(glCreateShader(_shaderType)),
		src()
	{}

	ShaderProgram(GLenum _shaderType, const char *_filename) :
        
		shaderType(_shaderType),
		shaderId(glCreateShader(_shaderType)),
		src(_filename)
	{
		BuildShader();
	}

	ShaderProgram(GLenum _shaderType, char *_buffer, size_t _length) :
        
		shaderType(_shaderType),
		shaderId(glCreateShader(_shaderType)),
		src(_buffer, _length)
	{
		BuildShader();
	}

	void BuildShader()
	{
		LoadSource();
		Compile();
	}

public:
	~ShaderProgram()
	{
		glDeleteShader(shaderId);
	}

	void LoadSourceFromFile(const char *_filename)
	{
		src.LoadFromFile(_filename);
		LoadSource();
	}
	
	void LoadSourceFromBuffer(char *_buffer, size_t _len)
	{
		src.LoadFromBuffer(_buffer, _len);
		LoadSource();
	}

	GLint Compile()
	{
		glCompileShader(shaderId);
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
		return status;
	}

	void GetInfoLog(char buff[1024])
	{
		glGetShaderInfoLog(shaderId, 1024, NULL, buff);
	}

	void ReleaseShader()
	{
		glDeleteShader(shaderId);
	}

	GLint GetCompileStatus()
	{
		return status;
	}

	GLuint GetShaderId()
	{
		return shaderId;
	}

	GLuint operator()()
	{
		return shaderId;
	}
};

class VertexProgram :
	public ShaderProgram
{
public:
	VertexProgram() :
		ShaderProgram(GL_VERTEX_SHADER)
	{}

	VertexProgram(const char *_filename) :
		ShaderProgram(GL_VERTEX_SHADER, _filename)
	{}

	VertexProgram(char *_buffer, size_t _length) :
		ShaderProgram(GL_VERTEX_SHADER, _buffer, _length)
	{}
};

class FragmentProgram :
	public ShaderProgram
{
public:
	FragmentProgram() :
		ShaderProgram(GL_FRAGMENT_SHADER)
	{}

	FragmentProgram(const char *_filename) :
		ShaderProgram(GL_FRAGMENT_SHADER, _filename)
	{}

	FragmentProgram(char *_buffer, size_t _length) :
		ShaderProgram(GL_FRAGMENT_SHADER, _buffer, _length)
	{}
};

class GpuProgram
{
protected:
	GLuint programId;
	VertexProgram *vxProgram;
	FragmentProgram *ftProgram;
	GLint status;

public:
	GpuProgram() :
        
		programId(glCreateProgram()),
		vxProgram(new VertexProgram()),
		ftProgram(new FragmentProgram())
	{}

	GpuProgram(const char *_vxFname, const char *_ftFname) :
        
		programId(glCreateProgram()),
		vxProgram(new VertexProgram(_vxFname)),
		ftProgram(new FragmentProgram(_ftFname))
	{
		BuildProgram();
	}

	GpuProgram(char *_vxBuffer, size_t _vxLen, char *_ftBuffer, size_t _ftLen) :
        
		programId(glCreateProgram()),
		vxProgram(new VertexProgram(_vxBuffer, _vxLen)),
		ftProgram(new FragmentProgram(_ftBuffer, _ftLen))
	{
		BuildProgram();
	}

	~GpuProgram()
    {
		vxProgram->ReleaseShader();
		ftProgram->ReleaseShader();
		glDeleteProgram(programId);
		delete vxProgram;
		delete ftProgram;
	}

	void UseProgram() const
	{
		glUseProgram(programId);
	}

	void LoadVertexShaderFromFile(const char *_vxFname)
	{
		vxProgram->LoadSourceFromFile(_vxFname);
	}
	
	void LoadFragmentShaderFromFile(const char *_ftFname)
	{
		ftProgram->LoadSourceFromFile(_ftFname);
	}

	void LoadShadersFromFiles(const char *_vxFname, const char *_ftFname)
	{
		LoadVertexShaderFromFile(_vxFname);
		LoadFragmentShaderFromFile(_ftFname);
	}

	void LoadVertexShaderObject(VertexProgram *_vxProgram)
	{
		vxProgram = _vxProgram;
	}

	void LoadFragmentShaderObject(FragmentProgram *_ftProgram)
	{
		ftProgram = _ftProgram;
	}

	GLint CompileVertexShader()
	{
		return vxProgram->Compile();
	}
	
	GLint CompileFragmentShader()
	{
		return ftProgram->Compile();
	}

	void CompileShaders()
	{
		CompileVertexShader();
		CompileFragmentShader();
	}
	
	void AttachShaders()
	{
		glAttachShader(programId, vxProgram->GetShaderId());
		glAttachShader(programId, ftProgram->GetShaderId());
	}

	void LinkProgram()
	{
		glLinkProgram(programId);

		glGetProgramiv(programId, GL_LINK_STATUS, &status);
	}

	void BuildProgram()
	{
		if(vxProgram->GetCompileStatus() != GL_TRUE)
		{
			vxProgram->Compile();
		}

		if(vxProgram->GetCompileStatus() != GL_TRUE)
		{
			char b[1024];
			vxProgram->GetInfoLog(b);
		}

		if(ftProgram->GetCompileStatus() != GL_TRUE)
		{
			ftProgram->Compile();
		}

		if(ftProgram->GetCompileStatus() != GL_TRUE)
		{
			char b[1024];
			ftProgram->GetInfoLog(b);
		}
		
		AttachShaders();
		LinkProgram();
		
		//if(status != GL_FALSE)
		//{
		//	vxProgram->ReleaseShader();
		//	ftProgram->ReleaseShader();
		//}
	}

	GLuint GetProgramId() const
	{
		return programId;
	}

	void Uniform1i(const char *name, GLint value) const
	{
		GLint location = glGetUniformLocation(GetProgramId(), name);
		glUniform1i(location, value);
	}

	void Uniform4fv(const char *name, GLsizei count, GLfloat *value) const
	{
		GLint location = glGetUniformLocation(GetProgramId(), name);
		glUniform4fv(location, count, value);
	}

	void Uniform3fv(const char *name, GLsizei count, GLfloat *value) const
	{
		GLint location = glGetUniformLocation(GetProgramId(), name);
		glUniform3fv(location, count, value);
	}

	void UniformMatrix4fv(const char *name, GLsizei count, GLboolean transpose, GLfloat *value) const
	{
		GLint location = glGetUniformLocation(GetProgramId(), name);
		glUniformMatrix4fv(location, count, transpose, value);
	}

	void UniformMatrix3fv(const char *name, GLsizei count, GLboolean transpose, GLfloat *value) const
	{
		GLint location = glGetUniformLocation(GetProgramId(), name);
		glUniformMatrix3fv(location, count, transpose, value);
	}

};

class DefaultProgram :
	public GpuProgram
{
public:
	struct _Attrib {
	protected:
		GLint position;
		GLint normal;
		GLint texture;
        
	public:
		_Attrib(GLuint programId)
		{
			position = glGetAttribLocation(programId, "position");
			normal   = glGetAttribLocation(programId, "normal");
		}

		GLint GetPositionLocation() const
		{
			return position;
		}
		
		GLint GetNormalLocation() const
		{
			return normal;
		}
	};

	struct _Uniform {
	public:
		GLint ambientColor;
		GLint diffuseColor;
		GLint specularColor;

		GLint ambientLight;
		GLint diffuseLight;
		GLint specularLight;

		GLint vLightPosition;
		GLint mvpMatrix;
		GLint mvMatrix;
		GLint normalMatrix;

	public:
        _Uniform(GLuint programId)
        {
			ambientColor  = glGetUniformLocation(programId, "ambientColor");
			diffuseColor  = glGetUniformLocation(programId, "diffuseColor");
			specularColor = glGetUniformLocation(programId, "specularColor");

			ambientLight  = glGetUniformLocation(programId, "ambientLight");
			diffuseLight  = glGetUniformLocation(programId, "diffuseLight");
			specularLight = glGetUniformLocation(programId, "specularLight");

			vLightPosition = glGetUniformLocation(programId, "vLightPosition");
			mvpMatrix      = glGetUniformLocation(programId, "mvpMatrix");
			mvMatrix       = glGetUniformLocation(programId, "mvMatrix");
			normalMatrix   = glGetUniformLocation(programId, "normalMatrix");
		}

		GLint GetAmbientColorLocation() const
		{
			return ambientColor;
		}
		GLint GetDiffuseColorLocation() const
		{
			return diffuseColor;
		}
		GLint GetSpecularColorLocation() const
		{
			return specularColor;
		}
		GLint GetAmbientLightLocation() const
		{
			return ambientLight;
		}
		GLint GetDiffuseLightLocation() const
		{
			return diffuseLight;
		}
		GLint GetSpecularLightLocation() const
		{
			return specularLight;
		}
		GLint GetVLightPositionLocation() const
		{
			return vLightPosition;
		}
		GLint GetMVPMatrixLocation() const
		{
			return mvpMatrix;
		}
		GLint GetMVMatrixLocation() const
		{
			return mvMatrix;
		}
		GLint GetNormalMatrixLocation() const
		{
			return normalMatrix;
		}
	};

public:
	_Attrib Attrib;
	_Uniform Uniform;

	DefaultProgram() :
		GpuProgram("Shaders/shader.vp", "Shaders/shader.fp"),
		Attrib(programId),
		Uniform(programId)
	{}
};

class DefaultProgramTextured :
	public GpuProgram
{
public:
	struct _Attrib :
		DefaultProgram::_Attrib
	{
	public:
		GLint texture;

		_Attrib(GLuint programId) :
			DefaultProgram::_Attrib(programId),
			texture(glGetAttribLocation(programId, "textureCoords"))
		{}

	public:
		GLint GetTextureLocation() const
		{
			return texture;
		}
	};

	struct _Uniform :
		DefaultProgram::_Uniform
	{
	public:
		GLint colorMap;
		_Uniform(GLuint programId) :
			DefaultProgram::_Uniform(programId),
			colorMap(glGetUniformLocation(programId, "colorMap"))
		{}

	public:
		GLint GetColorMapLocation() const
		{
				return colorMap;
		}
	};
public:
	_Attrib Attrib;
	_Uniform Uniform;

	DefaultProgramTextured() :
		GpuProgram("Shaders/shader_tex.vp", "Shaders/shader_tex.fp"),
		Attrib(programId),
		Uniform(programId)
	{}
};
