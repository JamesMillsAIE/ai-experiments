#pragma once

#include <glm/vec2.hpp>

namespace aie
{
	class Renderer;
}

using glm::vec2;

using aie::Renderer;

class Actor
{
	friend class ActorManager;

public:
	vec2 position;
	vec2 scale;
	float rotation;

protected:
	Actor(vec2 position = {}, vec2 scale = { 1.f, 1.f }, float rotation = 0.f);
	virtual ~Actor();

protected:
	virtual void BeginPlay();

	virtual void Tick();
	virtual void Render(Renderer* renderer);

	virtual void EndPlay();

};