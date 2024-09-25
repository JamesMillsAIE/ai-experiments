#include "ActorManager.h"

#include "Actor.h"

void ActorManager::Spawn(Actor* actor)
{
	if(std::ranges::find(m_actors, actor) != m_actors.end())
	{
		return;
	}

	m_changes.emplace_back(&ActorManager::AddActor, actor);
}

void ActorManager::Destroy(Actor* actor)
{
	if (std::ranges::find(m_actors, actor) == m_actors.end())
	{
		return;
	}

	m_changes.emplace_back(&ActorManager::RemoveActor, actor);
}

ActorManager::ActorManager() = default;

ActorManager::~ActorManager()
{
	for(auto& actor : m_actors)
	{
		delete actor;
	}

	m_actors.clear();
}

void ActorManager::Tick()
{
	for(auto& [change, actor] : m_changes)
	{
		(this->*change)(actor);
	}

	m_changes.clear();

	for(auto& actor : m_actors)
	{
		actor->Tick();
	}
}

void ActorManager::Render(Renderer* renderer) const
{
	for(auto& actor : m_actors)
	{
		actor->Render(renderer);
	}
}

void ActorManager::AddActor(Actor* actor)
{
	m_actors.emplace_back(actor);
	actor->BeginPlay();
}

void ActorManager::RemoveActor(Actor* actor)
{
	m_actors.remove(actor);
	actor->EndPlay();

	delete actor;
}
