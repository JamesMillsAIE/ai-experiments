#pragma once

#include <string>

#include "Global.h"

using std::string;

// a class for wrapping up an opengl texture image
class Texture
{
public:
	enum EFormat : unsigned int
	{
		R = 1,
		RG,
		RGB,
		RGBA
	};

public:
	Texture();
	Texture(const string& filename);
	Texture(unsigned int width, unsigned int height, EFormat format, const unsigned char* pixels = nullptr);
	virtual ~Texture();

public:
	// load a jpg, bmp, png or tga
	bool Load(string filename);

	// creates a texture that can be filled in with pixels
	void Create(unsigned int width, unsigned int height, EFormat format, const unsigned char* pixels = nullptr);

	// returns the filename or "none" if not loaded from a file
	string GetFilename() const;

	// binds the texture to the specified slot
	void Bind(unsigned int slot) const;

	// returns the opengl texture handle
	unsigned int GetHandle() const;

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	unsigned int GetFormat() const;
	const unsigned char* GetPixels() const;

	uint GetPixelAt(uint x, uint y) const;

protected:
	string m_filename;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_glHandle;
	unsigned int m_format;
	unsigned char* m_loadedPixels;
};