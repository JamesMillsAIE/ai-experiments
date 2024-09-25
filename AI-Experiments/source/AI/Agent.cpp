#include "Agent.h"

#include <aie/bootstrap/Application.h>
#include <aie/bootstrap/Random.h>
#include <aie/bootstrap/Renderer.h>
#include <aie/bootstrap/Window.h>

#include <glm/geometric.hpp>

#include "Global.h"

using aie::Random;
using aie::Window;

Agent::Agent(const AgentAttributes attributes, string name)
	: attributes{ attributes }, name{ std::move(name) }
{
	scale = vec2{ attributes.size };

	Random* rand = GetRandom();
	Window* window = GetWindow();

	position =
	{
		rand->Range(attributes.size, (float)(window->GetWidth()) - attributes.size),
		rand->Range(attributes.size, (float)(window->GetHeight()) - attributes.size),
	};
}

void Agent::Render(Renderer* renderer)
{
	renderer->DrawCircle(GlmToAie(position), length(scale), attributes.color);
}
