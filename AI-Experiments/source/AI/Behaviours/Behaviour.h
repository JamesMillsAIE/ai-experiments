#pragma once

#include <Core/Debugging/Debugger.h>

using Debugging::EVerbosity;

class AiBrain;
class Renderer2D;

class Behaviour
{
	friend AiBrain;

public:
	Behaviour();
	virtual ~Behaviour();

public:
	virtual void Tick() = 0;
	virtual void Render();

	virtual void RenderDebug(Renderer2D* renderer, EVerbosity verbosity) const = 0;
	virtual void HandleImGui(EVerbosity verbosity);

protected:
	AiBrain* m_owner;

};