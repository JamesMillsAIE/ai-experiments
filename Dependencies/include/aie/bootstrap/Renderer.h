#pragma once

#include <array>

#include <aie/bootstrap/Bootstrap2D.h>
#include <aie/bootstrap/Color.h>
#include <aie/bootstrap/Rectangle.h>
#include <aie/bootstrap/Vector2.h>

using std::array;

namespace aie
{
	class Texture;
	class Font;

	constexpr int TEXTURE_STACK_SIZE = 16;
	constexpr size_t MAX_SPRITES = 512;

	class DLL Renderer
	{
		friend class Window;

	public:
		static Renderer* Get();

	public:
		// all draw calls must occur between a begin / end pair
		void Begin();
		void End();

		// simple shape rendering
		void DrawRect(Vector2 pos, Vector2 size, Color color);
		void DrawRect(Rectangle rectangle, Color color);
		void DrawRectLines(Vector2 pos, Vector2 size, float thickness, Color color);


		void DrawCircle(Vector2 pos, float radius, Color color);
		void DrawCircleLines(Vector2 pos, float radius, float thickness, Color color);

		// if texture is nullptr then it renders a coloured sprite
		// depth is in the range [0,100] with lower being closer to the viewer
		void DrawTexture(Texture* texture, Vector2 pos, Color color);
		void DrawTexture(Texture* texture, Vector2 pos, Vector2 size, Color color);
		void DrawTexture(Texture* texture, Vector2 pos, Vector2 size, float rotation, Vector2 origin, Color color);
		void DrawTexture(Texture* texture, Rectangle source, Rectangle destination, float rotation, Vector2 origin, Color color);
		
		// draws a simple coloured line with a given thickness
		// depth is in the range [0,100] with lower being closer to the viewer
		void DrawLine(Vector2 start, Vector2 end, float thickness, Color color);

		// draws simple text on the screen horizontally
		// depth is in the range [0,100] with lower being closer to the viewer
		void DrawText(const char* text, Vector2 pos, Color color);
		void DrawText(Font* font, const char* text, Vector2 pos, Color color);

		Font* GetDefaultFont();

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
		static Renderer* m_instance;

	private:
		// indicates in the middle of a begin / end pair
		bool m_renderBegun;

		// texture handling
		Texture* m_nullTexture;
		Texture* m_textureStack[TEXTURE_STACK_SIZE];
		int	m_fontTexture[TEXTURE_STACK_SIZE];
		unsigned int m_currentTexture;

		// default font
		Font* m_defaultFont;

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

	private:
		// helper method used to rotate sprites around a pivot
		void RotateAround(float inX, float inY, float& outX, float& outY, float sin, float cos) const;

		// helper methods used during drawing
		bool ShouldFlush(int additionalVertices = 0, int additionalIndices = 0) const;
		void FlushBatch();
		unsigned int PushTexture(Texture* texture);

		array<array<float, 4>, 4> ProjectionMatrix() const;

	private:
		Renderer();
		~Renderer();

	};
}
