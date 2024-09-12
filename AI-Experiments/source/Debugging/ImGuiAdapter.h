#pragma once

class ImGuiAdapter
{
public:
	void InitImGui() const;
	void ShutdownImGui() const;
	void ImGuiNewFrame() const;
	void ImGuiRender() const;

};