#pragma once

#include <list>
#include <map>
#include <string>

namespace aie
{
	class Renderer;
}

using std::list;
using std::map;
using std::pair;
using std::string;

using aie::Renderer;

class Level;

class LevelManager
{
	friend class AiApplication;

private:
	typedef void(LevelManager::* ListChange)(Level*);

public:
	void AddLevel(Level* level);
	void Open(const string& name);
	void Close(Level* level);

	list<Level*> GetActiveLevels();

private:
	list<pair<ListChange, Level*>> m_changes;
	map<string, Level*> m_levels;
	list<Level*> m_activeLevels;

private:
	LevelManager();
	~LevelManager();

private:
	void Tick();
	void Render(Renderer* renderer) const;

	void OpenLevel(Level* level);
	void CloseLevel(Level* level);

};

