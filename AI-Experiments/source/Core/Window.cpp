#include "Window.h"

#include <GL/glew.h>

#include <glfw/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Input.h"
#include "Renderer2D.h"

int Window::GetWidth() const
{
	return m_width;
}

int Window::GetHeight() const
{
	return m_height;
}

string Window::GetTitle() const
{
	return m_title;
}

vec4 Window::GetClearColor() const
{
	return m_clearColor;
}

void Window::SetVSync(const bool enabled) const
{
	glfwSwapInterval(enabled ? 1 : 0);
}

void Window::SetClearColor(const vec4 clearColor) const
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

Window::operator GLFWwindow* () const
{
	return m_context;
}

Window::Window(const int width, const int height, string title, const vec4 clearColor)
	: m_context{ nullptr }, m_width{ width }, m_height{ height }, m_title{ std::move(title) },
	m_clearColor{ clearColor }, m_isOpen{ true }
{
}

Window::~Window() = default;

bool Window::IsOpen() const
{
	return m_isOpen;
}

bool Window::Open()
{
	if (glfwInit() == GL_FALSE)
	{
		return false;
	}

	m_context = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
	if (m_context == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_context);
	glfwSwapInterval(0);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		glfwDestroyWindow(m_context);
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(m_context, [](GLFWwindow*, int w, int h) { glViewport(0, 0, w, h); });

	glClearColor(1, 1, 1, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// start input renderer
	Renderer2D::Create();

	//InitImGui();

	return true;
}

void Window::Close() const
{
	ShutdownImGui();
	Renderer2D::Destroy();

	glfwDestroyWindow(m_context);
	glfwTerminate();
}

void Window::NewFrame() const
{
	// clear ImGui
	ImGuiNewFrame();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Renderer2D::Get()->Begin();
}

void Window::EndFrame()
{
	Renderer2D::Get()->End();

	// Render ImGui last
	ImGuiRender();

	//present back buffer to the monitor
	glfwSwapBuffers(m_context);

	// should the game exit?
	m_isOpen = m_isOpen && glfwWindowShouldClose(m_context) == GLFW_FALSE;

	if(Input::Get()->WasKeyReleased(KeyEscape))
	{
		m_isOpen = false;
	}
}

void Window::InitImGui() const
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable GamePad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	GLFWwindow* window = glfwGetCurrentContext();
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void Window::ShutdownImGui() const
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Window::ImGuiNewFrame() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Window::ImGuiRender() const
{
	const ImGuiIO& io = ImGui::GetIO();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Tick and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backupCurrentContext);
	}
}
