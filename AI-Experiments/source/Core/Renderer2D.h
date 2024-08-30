#pragma once

class Texture;
class Font;

constexpr int TEXTURE_STACK_SIZE = 16;
constexpr size_t MAX_SPRITES = 512;

// a class for rendering 2D sprites and font
class Renderer2D
{
	friend class Window;

public:
	static Renderer2D* Get();

public:
	// all draw calls must occur between a begin / end pair
	void Begin();
	void End();

	// simple shape rendering
	void DrawBox(float xPos, float yPos, float width, float height, float rotation = 0.0f, float depth = 0.0f);
	void DrawCircle(float xPos, float yPos, float radius, float depth = 0.0f);

	// if texture is nullptr then it renders a coloured sprite
	// depth is in the range [0,100] with lower being closer to the viewer
	void DrawSprite(Texture* texture, float xPos, float yPos, float width = 0.0f, float height = 0.0f, float rotation = 0.0f, float depth = 0.0f, float xOrigin = 0.5f, float yOrigin = 0.5f);
	void DrawSpriteTransformed3x3(Texture* texture, float* transformMat3X3, float width = 0.0f, float height = 0.0f, float depth = 0.0f, float xOrigin = 0.5f, float yOrigin = 0.5f);
	void DrawSpriteTransformed4x4(Texture* texture, float* transformMat4x4, float width = 0.0f, float height = 0.0f, float depth = 0.0f, float xOrigin = 0.5f, float yOrigin = 0.5f);

	// draws a simple coloured line with a given thickness
	// depth is in the range [0,100] with lower being closer to the viewer
	void DrawLine(float x1, float y1, float x2, float y2, float thickness = 1.0f, float depth = 0.0f);

	// draws simple text on the screen horizontally
	// depth is in the range [0,100] with lower being closer to the viewer
	void DrawText(Font* font, const char* text, float xPos, float yPos, float depth = 0.0f);

	// sets the tint colour for all subsequent draw calls
	void SetRenderColour(float r, float g, float b, float a = 1.0f);
	void SetRenderColour(unsigned int colour);

	// can be used to set the texture coordinates of sprites using textures
	// for all subsequent drawSprite calls
	void SetUVRect(float uvX, float uvY, float uvW, float uvH);

	// specify the camera position
	void SetCameraPos(float x, float y);
	void GetCameraPos(float& x, float& y);

private:
	// sprite handling
	struct SBVertex
	{
		float pos[4];
		float color[4];
		float texCoord[2];
	};

private:
	static void Create();
	static void Destroy();

private:
	static Renderer2D* m_instance;

private:
	// indicates in the middle of a begin/end pair
	bool m_renderBegun;

	// the camera position
	float m_cameraX;
	float m_cameraY;

	// texture handling
	Texture* m_nullTexture;
	Texture* m_textureStack[TEXTURE_STACK_SIZE];
	int	m_fontTexture[TEXTURE_STACK_SIZE];
	unsigned int m_currentTexture;

	// texture coordinate information
	float m_uvX;
	float m_uvY;
	float m_uvW;
	float m_uvH;

	// represents colour in red, green, blue and alpha 0.0-1.0 range
	float m_r;
	float m_g;
	float m_b;
	float m_a;

	// data used for opengl to draw the sprites (with padding)
	SBVertex m_vertices[MAX_SPRITES * 4];
	unsigned short m_indices[MAX_SPRITES * 6];

	short m_currentVertex;
	int m_currentIndex;

	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;

	// shader used to render sprites
	unsigned int m_shader;

	// data used for a camera
	float m_projectionMatrix[16];

private:
	// helper method used to rotate sprites around a pivot
	void RotateAround(float inX, float inY, float& outX, float& outY, float sin, float cos) const;

	// helper methods used during drawing
	bool ShouldFlush(int additionalVertices = 0, int additionalIndices = 0) const;
	void FlushBatch();
	unsigned int PushTexture(Texture* texture);

private:
	Renderer2D();
	~Renderer2D();

};