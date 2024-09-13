#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <imgui.h>
#include <string>
#include <vector>

using glm::vec3;
using glm::vec4;

using std::vector;
using std::string;

class ImGuiAdapter
{
public:
	void InitImGui() const;
	void ShutdownImGui() const;
	void ImGuiNewFrame() const;
	void ImGuiRender() const;

	bool BeginCollapsingGroup(const string& title);
	void EndCollapsingGroup();

	void BeginDisabledGroup(bool disabled);
	void EndDisabledGroup();

	bool ColorEditField(const string& label, vec3& color);
	bool ColorEditField(const string& label, vec4& color);

	template<typename ENUM>
	void EnumField(const string& label, ENUM& value, vector<const char*> arr);

};

template <typename ENUM>
void ImGuiAdapter::EnumField(const string& label, ENUM& value, vector<const char*> arr)
{
	int verbosity = static_cast<int>(value);
	ImGui::Combo(label.c_str(), &verbosity, arr.data(), 2);
	value = static_cast<ENUM>(verbosity);
}