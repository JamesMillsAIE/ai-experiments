#include "MainLevel.h"

#include "Global.h"

#include <fstream>

#include <aie/bootstrap/Random.h>

#include "AiApplication.h"

#include "Actors/ActorManager.h"
#include "Actors/NameGenerator.h"

using std::fstream;

MainLevel::MainLevel()
	: Level{ MAIN_LEVEL }
{
}

MainLevel::~MainLevel() = default;

void MainLevel::BeginPlay()
{
	fstream agentFile("resources/configuration/AgentConfig.json");

	Json j = Json::parse(agentFile);
	m_counts = j["agent_counts"];
	PopulateAttribLookupTable(j);

	for(auto& [type, count] : m_counts)
	{
		for (int i = 0; i < count; i++)
		{
			GetActorManager()->Spawn(new Agent(m_attribLookup[type], NameGenerator::NewName()));
		}
	}
}

void MainLevel::EndPlay()
{

}

void MainLevel::PopulateAttribLookupTable(Json& j)
{
	vector<AgentAttributes> attributes = j["agents"];

	for (auto& attrib : attributes)
	{
		if (!m_attribLookup.contains(attrib.type))
		{
			m_attribLookup[attrib.type] = attrib;
		}
	}
}