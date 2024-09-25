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
	virtual void BeginPlay() = 0;

	virtual void Tick();
	virtual void Render(Renderer* renderer);

	virtual void EndPlay() = 0;

private:
	string m_name;

};
