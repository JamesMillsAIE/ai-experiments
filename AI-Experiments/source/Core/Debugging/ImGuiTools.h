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

namespace Debugging
{
	class ImGuiTools
	{
	public:
		static bool BeginCollapsingGroup(const string& title);
		static void EndCollapsingGroup();

		static void BeginDisabledGroup(bool disabled);
		static void EndDisabledGroup();

		static bool ColorEditField(const string& label, vec3& color);
		static bool ColorEditField(const string& label, vec4& color);

		template<typename ENUM>
		static void EnumField(const string& label, ENUM& value, vector<const char*> arr);

	};

	template <typename ENUM>
	void ImGuiTools::EnumField(const string& label, ENUM& value, const vector<const char*> arr)
	{
		int verbosity = static_cast<int>(value);
		ImGui::Combo("Verbosity", &verbosity, arr.data(), 2);
		value = static_cast<ENUM>(verbosity);
	}
}
