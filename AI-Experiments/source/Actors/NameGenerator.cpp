#include "NameGenerator.h"

#include <fstream>

#include <aie/bootstrap/Application.h>
#include <aie/bootstrap/Random.h>

using std::fstream;

using aie::Random;

vector<string> NameGenerator::m_firstNames;
vector<string> NameGenerator::m_lastNames;

bool NameGenerator::m_initialised = false;

string NameGenerator::NewName()
{
	if(!m_initialised)
	{
		fstream file("resources/configuration/Names.json");
		Json j = Json::parse(file);

		m_firstNames = j["names"]["first"];
		m_lastNames = j["names"]["last"];

		m_initialised = true;
	}

	Random* rand = GetRandom();

	string first = m_firstNames[rand->Range(0, (int)(m_firstNames.size()) - 1)];
	string last = m_lastNames[rand->Range(0, (int)(m_lastNames.size()) - 1)];

	return first + " " + last;
}
