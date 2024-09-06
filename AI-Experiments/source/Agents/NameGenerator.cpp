#include "NameGenerator.h"

#include <fstream>

#include "Core/Structures/Random.h"

using std::ifstream;

vector<string> NameGenerator::m_firstNames;
vector<string> NameGenerator::m_lastNames;

void NameGenerator::Load()
{
	LoadNames("resources/configuration/first_names.txt", m_firstNames);
	LoadNames("resources/configuration/last_names.txt", m_lastNames);
}

string NameGenerator::NewName(const Random* random)
{
	return m_firstNames[random->Range(0, static_cast<int>(m_firstNames.size() - 1))] + 
		" " + m_lastNames[random->Range(0, static_cast<int>(m_lastNames.size() - 1))];
}

void NameGenerator::LoadNames(const string& file, vector<string>& names)
{
	ifstream stream(file.c_str());

	if(stream.is_open())
	{
		string line;
		while(std::getline(stream, line))
		{
			names.emplace_back(line);
		}
	}

	stream.close();
}
