#pragma once

#include <aie/bootstrap/Application.h>

using aie::Application;

class ImGuiAdapter;

class AiApplication final : public Application
{
public:
	AiApplication();

protected:
	void Init() override;
	void Tick() override;
	void Render() override;
	void Shutdown() override;

private:
	ImGuiAdapter* m_imGui;

};

