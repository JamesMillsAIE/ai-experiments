#pragma once

#include <aie/bootstrap/Application.h>

using aie::Application;

class ImGuiAdapter;

extern ImGuiAdapter* GetImGuiAdapter();

class AiApplication final : public Application
{
public:
	friend ImGuiAdapter* ::GetImGuiAdapter();

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

};

