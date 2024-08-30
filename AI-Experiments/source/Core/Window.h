#pragma once

#include <string>

#include <glm/vec4.hpp>

using std::string;

using glm::vec4;

struct GLFWwindow;

class Window
{
	friend class Game;

public:
	int GetWidth() const;
	int GetHeight() const;
	string GetTitle() const;
	vec4 GetClearColor() const;

	void SetVSync(bool enabled) const;
	void SetClearColor(vec4 clearColor) const;

public:
	operator GLFWwindow* () const;

private:
	Window(int width, int height, string title, vec4 clearColor);
	~Window();

private:
	GLFWwindow* m_context;

	int m_width;
	int m_height;
	string m_title;
	vec4 m_clearColor;

	bool m_isOpen;

private:
	bool IsOpen() const;

	bool Open();
	void Close() const;

	void NewFrame() const;
	void EndFrame();

	void InitImGui() const;
	void ShutdownImGui() const;
	void ImGuiNewFrame() const;
	void ImGuiRender() const;

};