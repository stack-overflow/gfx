#pragma once
#include <vector>
#include <memory>
#include <limits>

#include "OpenGLSystem.h"

class Texture
{
public:
	~Texture();
	unsigned int GetName() const;
	unsigned int GetType() const;
	unsigned int GetTextureUnit() const;
	unsigned int GetSampler() const;

	void Use();
	void Unuse();
	void Unload();
	void SetTextureUnit(unsigned int _textureUnit);
	void GenerateMipmap();
	void SetMaxAnisotropy();
	void SetMagFilter(unsigned int _magFilter);
	void SetMinFilter(unsigned int _minFilter);
	void SetMagMinFilters(unsigned int _magFilter, unsigned int _minFilter);
	void SetWrapST(unsigned int _wrapS, unsigned int _wrapT);

protected:
	Texture();
	Texture(unsigned int _textureType);
	Texture(unsigned int _textureType, unsigned int _textureUnit);
	

	void *LoadBmpFile(const char *filename);

protected:
	OpenGLSystem *m_OpenGL;

	unsigned int m_textureUnit;
	unsigned int m_textureName;
	unsigned int m_textureType;

	int m_width;
	int m_height;
	int m_size;

	bool m_loaded;
};

class Texture2D :
	public Texture
{
public:
	Texture2D();
	Texture2D(const char *_filename, unsigned int _textureUnit);
	~Texture2D();

	bool LoadTextureBMPFromFile(const char *filename, unsigned int _textureUnit);

private:
	bool CreateTexture(const void *pixels);
};

class TextureCubeMap :
	public Texture
{
public:
	TextureCubeMap();
	TextureCubeMap(unsigned int _textureUnit);
	TextureCubeMap(const char *_filenames[6], unsigned int _textureUnit);
	~TextureCubeMap();

	bool LoadTextureFaceFromBMPFile(
			const char *_filename,
			unsigned int _face);

private:
	bool CreateTextureFace(unsigned int _face, const void *_pixels);
	void SetFacesUnloaded();

private:
	bool m_faceLoaded[6];
};

class TextureManager
{
public:
    TextureManager()
    {}

    unsigned int LoadTexture2D(const char *_filename)
    {
        Texture2D *tex2D = new Texture2D(_filename, GL_TEXTURE0);
        if (tex2D != NULL)
        {
            return InsertTexture(tex2D);
        }
#undef max
        return std::numeric_limits<unsigned int>::max();
    }

    unsigned int InsertTexture(Texture *_texture)
    {
        int retIndex = m_textures.size();
        m_textures.push_back(std::unique_ptr<Texture>(_texture));

        return retIndex;
    }

    __forceinline
    Texture *const GetTexture(unsigned int _mgrId) const
    {
        return m_textures[_mgrId].get();
    }
private:
    OpenGLSystem *m_OpenGL;
    std::vector<std::unique_ptr<Texture>> m_textures;
};