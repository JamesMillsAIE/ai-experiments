#include "LevelManager.h"

#include <ranges>

#include "Level.h"

void LevelManager::AddLevel(Level* level)
{
	if(m_levels.contains(level->m_name))
	{
		return;
	}

	m_levels[level->m_name] = level;
}

void LevelManager::Open(const string& name)
{
	if(!m_levels.contains(name))
	{
		return;
	}

	Level* level = m_levels[name];
	if(std::ranges::find(m_activeLevels, level) == m_activeLevels.end())
	{
		return;
	}

	m_changes.emplace_back(&LevelManager::OpenLevel, level);
}

void LevelManager::Close(Level* level)
{
	if (std::ranges::find(m_activeLevels, level) != m_activeLevels.end())
	{
		return;
	}

	m_changes.emplace_back(&LevelManager::CloseLevel, level);
}

list<Level*> LevelManager::GetActiveLevels()
{
	return m_activeLevels;
}

LevelManager::LevelManager() = default;

LevelManager::~LevelManager()
{
	for(auto& level : m_levels | std::views::values)
	{
		delete level;
	}

	m_levels.clear();
	m_activeLevels.clear();
}

void LevelManager::Tick()
{
	for(auto& [change, level] : m_changes)
	{
		(this->*change)(level);
	}

	m_changes.clear();

	for(auto& level : m_activeLevels)
	{
		level->Tick();
	}
}

void LevelManager::Render(Renderer* renderer) const
{
	for (auto& level : m_activeLevels)
	{
		level->Render(renderer);
	}
}

void LevelManager::OpenLevel(Level* level)
{
	m_activeLevels.emplace_back(level);
	level->BeginPlay();
}

void LevelManager::CloseLevel(Level* level)
{
	level->EndPlay();
	m_activeLevels.remove(level);
}
