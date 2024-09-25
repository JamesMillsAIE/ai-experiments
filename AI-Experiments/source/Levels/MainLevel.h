#pragma once

#include <string>
#include <vector>

#include "Level.h"

#include "AI/Agent.h"

namespace aie
{
	class Random;
}

using std::pair;
using std::map;
using std::string;
using std::vector;

using aie::Random;

struct AgentCount
{
	EAgentType type;
	int count;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(AgentCount, type, count)
};

class MainLevel final : public Level
{
public:
	MainLevel();
	~MainLevel() override;

protected:
	void BeginPlay() override;
	void EndPlay() override;

private:
	vector<AgentCount> m_counts;

	map<EAgentType, AgentAttributes> m_attribLookup;

private:
	void PopulateAttribLookupTable(Json& j);

};

