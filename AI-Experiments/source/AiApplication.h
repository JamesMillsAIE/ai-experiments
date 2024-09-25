#pragma once

#include <aie/bootstrap/Application.h>

using aie::Application;

class ImGuiAdapter;
class ActorManager;
class LevelManager;

extern ImGuiAdapter* GetImGuiAdapter();
extern ActorManager* GetActorManager();
extern LevelManager* GetLevelManager();

class AiApplication final : public Application
{
public:
	friend ImGuiAdapter* ::GetImGuiAdapter();
	friend ActorManager* ::GetActorManager();
	friend LevelManager* ::GetLevelManager();

public:
	AiApplication();
	~AiApplication() override;

protected:
	static AiApplication* GetInstance();

protected:
	void Init() override;
	void Tick() override;
	void Render() override;
	void Shutdown() override;

private:
	ImGuiAdapter* m_imGui;
	ActorManager* m_actorManager;
	LevelManager* m_levelManager;

};

