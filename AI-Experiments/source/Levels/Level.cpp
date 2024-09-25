#include "Level.h"

Level::Level(string name)
	: m_name{ std::move(name) }
{
	
}

Level::~Level() = default;

string Level::Name()
{
	return m_name;
}

void Level::BeginPlay() { }

void Level::Tick() { }

void Level::Render(Renderer* renderer) { }

void Level::EndPlay() { }