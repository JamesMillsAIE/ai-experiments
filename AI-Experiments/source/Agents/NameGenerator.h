#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

class Random;

class NameGenerator
{
public:
	static void Load();
	static string NewName(const Random* random);

private:
	static vector<string> m_firstNames;
	static vector<string> m_lastNames;

private:
	static void LoadNames(const string& file, vector<string>& names);

};