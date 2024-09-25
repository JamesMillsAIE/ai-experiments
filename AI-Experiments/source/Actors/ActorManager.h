#pragma once

#include <list>

namespace aie
{
	class Renderer;
}

using std::list;
using std::pair;

using aie::Renderer;

class Actor;

class ActorManager
{
	friend class AiApplication;

private:
	typedef void(ActorManager::* ListChange)(Actor*);

public:
	void Spawn(Actor* actor);
	void Destroy(Actor* actor);

private:
	list<pair<ListChange, Actor*>> m_changes;
	list<Actor*> m_actors;

private:
	ActorManager();
	~ActorManager();

private:
	void Tick();
	void Render(Renderer* renderer) const;

	void AddActor(Actor* actor);
	void RemoveActor(Actor* actor);

};