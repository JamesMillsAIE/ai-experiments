#pragma once

#include <string>

#include <aie/bootstrap/Color.h>

#include "Actors/Actor.h"

#include "Utilities/JsonAdls.h"

using std::string;

enum class EAgentType
{
	Invalid = -1,
	Barbarian,
	Civilian,
	Guard
};

NLOHMANN_JSON_SERIALIZE_ENUM(EAgentType,
	{
		{ EAgentType::Invalid, nullptr },
		{ EAgentType::Barbarian, "Barbarian" },
		{ EAgentType::Civilian, "Civilian" },
		{ EAgentType::Guard, "Guard" }
	});

class AgentAttributes
{
public:
	float speed;
	float size;
	EAgentType type;

	Color color;

public:
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(AgentAttributes, speed, size, type, color)

};

class Agent : public Actor
{
public:
	AgentAttributes attributes;
	string name;

public:
	Agent() = delete;
	Agent(AgentAttributes attributes, string name);

protected:
	void Render(Renderer* renderer) override;

};