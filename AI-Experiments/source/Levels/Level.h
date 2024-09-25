#pragma once

#include <string>

namespace aie
{
	class Renderer;
}

using std::string;

using aie::Renderer;

class Level
{
	friend class LevelManager;

public:
	Level() = delete;
	Level(string name);
	virtual ~Level();

public:
	string Name();

protected:
	virtual void BeginPlay();

	virtual void Tick();
	virtual void Render(Renderer* renderer);

	virtual void EndPlay();

private:
	string m_name;

};
