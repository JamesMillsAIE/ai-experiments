#pragma once

#include <glm/vec2.hpp>

#include <string>

using glm::vec2;

using std::string;

class Texture;

class Sprite
{
public:
	explicit Sprite(const string& file);
	~Sprite();

public:
	void SetUV(float x, float y, float w, float h);
	void Draw(vec2 position, vec2 size) const;

private:
	Texture* m_texture;

	float m_uvX;
	float m_uvY;
	float m_uvW;
	float m_uvH;

};