#include "Actor.h"

Actor::Actor(const vec2 position, const vec2 scale, const float rotation)
	: position{ position }, scale{ scale }, rotation{ rotation }
{
}

Actor::~Actor() = default;

void Actor::BeginPlay() { }

void Actor::Tick() { }

void Actor::Render(Renderer* renderer) { }

void Actor::EndPlay() { }