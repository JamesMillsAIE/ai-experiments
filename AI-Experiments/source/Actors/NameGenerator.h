#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using std::string;
using std::vector;

using Json = nlohmann::json;

class NameGenerator
{
public:
	static string NewName();

private:
	static vector<string> m_firstNames;
	static vector<string> m_lastNames;

	static bool m_initialised;

};