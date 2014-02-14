#include "Texture.h"
#include "Utils.h"
#include <fstream>

Texture::Texture() :
	m_textureUnit(GL_TEXTURE0),
	m_textureName(0),
	m_textureType(0),
	m_width(0),
	m_height(0),
	m_size(0),
	m_loaded(false)
{}

Texture::Texture(unsigned int _textureType) :
	m_textureUnit(GL_TEXTURE0),
	m_textureName(0),
	m_textureType(_textureType),
	m_width(0),
	m_height(0),
	m_size(0),
	m_loaded(false)
{}

Texture::Texture(unsigned int _textureType, unsigned int _textureUnit) :
	m_textureUnit(_textureUnit),
	m_textureName(0),
	m_textureType(_textureType),
	m_width(0),
	m_height(0),
	m_size(0),
	m_loaded(false)
{}

void Texture::Use()
{
	glActiveTexture(m_textureUnit);
    glBindTexture(m_textureType, m_textureName);
}

void Texture::Unuse()
{
    glActiveTexture(m_textureUnit);
	glBindTexture(m_textureType, 0);
}

void Texture::Unload()
{
	if (m_loaded)
	{
		glDeleteTextures(1, &m_textureName);
		m_loaded = false;
	}
}

void Texture::SetTextureUnit(unsigned int _textureUnit)
{
	m_textureUnit = _textureUnit;
}

Texture::~Texture()
{
	Unload();
}

unsigned int Texture::GetName() const
{
	return m_textureName;
}

unsigned int Texture::GetTextureUnit() const
{
	return m_textureUnit;
}

unsigned int Texture::GetType() const
{
	return m_textureType;
}

unsigned int Texture::GetSampler() const
{
	return m_textureUnit - GL_TEXTURE0;
}

void Texture::SetMaxAnisotropy()
{
	float max;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max);
	glTexParameterf(m_textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, max);
}

void Texture::SetMagFilter(unsigned int _magFilter)
{
	glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, _magFilter);
}

void Texture::SetMinFilter(unsigned int _minFilter)
{
	glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, _minFilter);
}

void Texture::SetMagMinFilters(unsigned int _magFilter, unsigned int _minFilter)
{
	glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, _magFilter);
	glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, _minFilter);
}

void Texture::SetWrapST(unsigned int _wrapS, unsigned int _wrapT)
{
	glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, _wrapS);
	glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, _wrapT);
}

bool Texture2D::LoadTextureBMPFromFile(const char *filename, unsigned int _textureUnit)
{
	void *pixels = LoadBmpFile(filename);

	m_textureUnit = _textureUnit;

	if (!CreateTexture(pixels))
	{
		return false;
	}

	delete pixels;

	m_loaded = true;

	return true;
}

void *Texture::LoadBmpFile(const char *filename)
{
	char header[54];
	char *data;
	unsigned int dataPos;

	std::ifstream inStream(filename, std::ios::binary);

	if(!inStream)
	{
		__DEBUG_ERR("Bad, bad filename.\n");
	}

	inStream.read(header, 54);
	if(inStream.gcount() < 54)
	{
		__DEBUG_ERR("Bad, bad, not a  BMP file\n");
	}

	if(header[0] != 'B' || header[1] != 'M')
	{
		__DEBUG_ERR("Bad, bad, not a  BMP file\n");
	}

	dataPos   = *(int*)&(header[0x0A]);
	m_size = *(int*)&(header[0x22]);
	m_width     = *(int*)&(header[0x12]);
	m_height    = *(int*)&(header[0x16]);

	if(m_size == 0)
	{
		m_size = m_width * m_height * 3;
	}

	if(dataPos == 0)
	{
		dataPos = 54;
	}

	data = new char[m_size];

	inStream.read(data, m_size);

	inStream.close();

	return data;
}

void Texture::GenerateMipmap()
{
	glGenerateMipmap(m_textureType);
}

bool Texture2D::CreateTexture(const void *pixels)
{
	glGenTextures(1, &m_textureName);

	if (m_textureName == 0)
	{
		return false;
	}

	glActiveTexture(m_textureUnit);

	glBindTexture(m_textureType, m_textureName);
	glTexImage2D(m_textureType, 0, GL_RGB, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);

	SetWrapST(
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_EDGE);

	SetMagMinFilters(
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR);

	SetMaxAnisotropy();

	GenerateMipmap();

	return true;
}

Texture2D::Texture2D() :
	Texture(GL_TEXTURE_2D)
{}

Texture2D::Texture2D(const char *_filename, unsigned int _textureUnit) :
	Texture(GL_TEXTURE_2D, _textureUnit)
{
	LoadTextureBMPFromFile(_filename, _textureUnit);
}

Texture2D::~Texture2D()
{
	Unload();
}

TextureCubeMap::TextureCubeMap() :
	Texture(GL_TEXTURE_CUBE_MAP)
{
	SetFacesUnloaded();
}

TextureCubeMap::TextureCubeMap(unsigned int _textureUnit) :
	Texture(GL_TEXTURE_CUBE_MAP, _textureUnit)
{
	SetFacesUnloaded();
}

TextureCubeMap::TextureCubeMap(const char *_filenames[6], unsigned int _textureUnit) :
	Texture(GL_TEXTURE_CUBE_MAP, _textureUnit)
{
	SetFacesUnloaded();

	int face;
	for (face = 0;
		 face < 6;
		 ++face)
	{
		if(!LoadTextureFaceFromBMPFile(_filenames[face], GL_TEXTURE_CUBE_MAP_POSITIVE_X + face))
		{
			break;
		}
	}

	if (face == 6)
	{
		m_loaded = true;
	}
}

bool TextureCubeMap::LoadTextureFaceFromBMPFile(
		const char *_filename,
		unsigned int _face)
{
	void *pixels;
	pixels = LoadBmpFile(_filename);

	if (!CreateTextureFace(_face, pixels))
	{
		m_faceLoaded[_face - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = false;
		return false;
	}

	m_faceLoaded[_face - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = true;

	return true;
}

bool TextureCubeMap::CreateTextureFace(unsigned int _face, const void *_pixels)
{
	if (m_textureName == 0)
	{
		glGenTextures(1, &m_textureName);
	}

	if (m_textureName == 0)
	{
		return false;
	}

	glActiveTexture(m_textureUnit);

	glBindTexture(m_textureType, m_textureName);
	glTexImage2D(_face, 0, GL_RGB, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, _pixels);

	SetWrapST(
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_EDGE);

	SetMagMinFilters(
		GL_LINEAR,
		GL_LINEAR);

	SetMaxAnisotropy();

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	GenerateMipmap();

	return true;
}

void TextureCubeMap::SetFacesUnloaded()
{
	m_faceLoaded[0] = m_faceLoaded[1] =
	m_faceLoaded[2] = m_faceLoaded[3] =
	m_faceLoaded[4] = m_faceLoaded[5] = false;
}

TextureCubeMap::~TextureCubeMap()
{
	Unload();
}
