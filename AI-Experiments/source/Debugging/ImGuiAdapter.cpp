// ReSharper disable CppMemberFunctionMayBeStatic
#include "ImGuiAdapter.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <GLFW/glfw3.h>

void ImGuiAdapter::InitImGui() const
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

void ImGuiAdapter::ShutdownImGui() const
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiAdapter::ImGuiNewFrame() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiAdapter::ImGuiRender() const
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

bool ImGuiAdapter::BeginCollapsingGroup(const string& title)
{
	ImGui::PushID(title.c_str());

	return ImGui::CollapsingHeader(title.c_str());
}

void ImGuiAdapter::EndCollapsingGroup()
{
	ImGui::PopID();
}

void ImGuiAdapter::BeginDisabledGroup(bool disabled)
{
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disabled);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * (disabled ? .5f : 1.f));
}

void ImGuiAdapter::EndDisabledGroup()
{
	ImGui::PopStyleVar();
	ImGui::PopItemFlag();
}

bool ImGuiAdapter::ColorEditField(const string& label, vec3& color)
{
	float colArr[] = { color.r, color.g, color.b };

	if (ImGui::ColorEdit3(label.c_str(), colArr))
	{
		color = vec3(colArr[0], colArr[1], colArr[2]);
		return true;
	}

	return false;
}

bool ImGuiAdapter::ColorEditField(const string& label, vec4& color)
{
	float colArr[] = { color.r, color.g, color.b, color.a };

	if (ImGui::ColorEdit4(label.c_str(), colArr))
	{
		color = vec4(colArr[0], colArr[1], colArr[2], colArr[3]);
		return true;
	}

	return false;
}
