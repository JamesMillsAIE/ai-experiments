#include "Agent.h"

Agent::Agent(const AgentAttributes attributes, string name)
	: attributes{ attributes }, name{ std::move(name) }
{
}