#include "Agent.h"

#include <aie/bootstrap/Renderer.h>

#include <glm/geometric.hpp>

#include "Global.h"

Agent::Agent(const AgentAttributes attributes, string name)
	: attributes{ attributes }, name{ std::move(name) }
{
	scale = vec2{ attributes.size } *.5f;
}

void Agent::Render(Renderer* renderer)
{
	renderer->DrawCircle(GlmToAie(position), length(scale), attributes.color);
}
