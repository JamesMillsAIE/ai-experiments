#include "Sprite.h"

#include "Renderer2D.h"
#include "Texture.h"

Sprite::Sprite(const string& file)
	: m_texture{ new Texture(file) }, m_uvX{ 0 }, m_uvY{ 0 }, m_uvW{ 1.f }, m_uvH{ 1.f }
{
}

Sprite::~Sprite()
{
	delete m_texture;
	m_texture = nullptr;
}

void Sprite::SetUV(const float x, const float y, const float w, const float h)
{
	m_uvX = x;
	m_uvY = y;
	m_uvW = w;
	m_uvH = h;
}

void Sprite::Draw(const vec2 position, const vec2 size) const
{
	if (Renderer2D* renderer = Renderer2D::Get(); m_texture && renderer)
	{
		const float texWidth = static_cast<float>(m_texture->GetWidth());
		const float texHeight = static_cast<float>(m_texture->GetHeight());

		renderer->SetUVRect(
			m_uvX / texWidth, m_uvY / texHeight,
			m_uvW / texWidth, m_uvH / texHeight
		);

		renderer->DrawSprite(m_texture, position, size);

		renderer->SetUVRect(0, 0, 1, 1);
	}
}
