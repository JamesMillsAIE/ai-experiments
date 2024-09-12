#pragma once

#include <string>

#include <aie/bootstrap/Bootstrap2D.h>
#include <aie/bootstrap/Color.h>

using std::string;

struct GLFWwindow;

namespace aie
{
	class DLL Window
	{
		friend class Application;

	public:
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
		string GetTitle() const;
		Color GetClearColor() const;

		void SetVSync(bool enabled) const;
		void SetClearColor(Color clearColor) const;

	public:
		operator GLFWwindow* () const;

	private:
		Window(unsigned int width, unsigned int height, string title, Color clearColor);
		~Window();

	private:
		GLFWwindow* m_context;

		unsigned int m_width;
		unsigned int m_height;
		string m_title;
		Color m_clearColor;

		bool m_isOpen;

	private:
		bool IsOpen() const;

		bool Open();
		void Close() const;

		void NewFrame() const;
		void EndFrame();

	};
}