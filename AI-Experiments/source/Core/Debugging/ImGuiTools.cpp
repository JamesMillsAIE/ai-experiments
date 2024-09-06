#include "ImGuiTools.h"

#include "imgui_internal.h"

namespace Debugging
{
	bool ImGuiTools::BeginCollapsingGroup(const string& title)
	{
		ImGui::PushID(title.c_str());

		bool open = ImGui::CollapsingHeader(title.c_str());

		return open;
	}

	void ImGuiTools::EndCollapsingGroup()
	{
		ImGui::PopID();
	}

	void ImGuiTools::BeginDisabledGroup(bool disabled)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disabled);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * (disabled ? .5f : 1.f));
	}

	void ImGuiTools::EndDisabledGroup()
	{
		ImGui::PopStyleVar();
		ImGui::PopItemFlag();
	}

	bool ImGuiTools::ColorEditField(const string& label, vec3& color)
	{
		float colArr[] = { color.r, color.g, color.b };

		if(ImGui::ColorEdit3(label.c_str(), colArr))
		{
			color = vec3(colArr[0], colArr[1], colArr[2]);
			return true;
		}

		return false;
	}

	bool ImGuiTools::ColorEditField(const string& label, vec4& color)
	{
		float colArr[] = { color.r, color.g, color.b, color.a };

		if (ImGui::ColorEdit4(label.c_str(), colArr))
		{
			color = vec4(colArr[0], colArr[1], colArr[2], colArr[3]);
			return true;
		}

		return false;
	}
}
