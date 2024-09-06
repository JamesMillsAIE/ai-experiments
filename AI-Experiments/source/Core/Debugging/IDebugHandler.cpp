#include "IDebugHandler.h"

#include "imgui.h"

namespace Debugging
{
	IDebugHandler::IDebugHandler()
		: m_isEnabled{ false }
	{
		Debugger::ObserveDebugTool(this);
	}

	IDebugHandler::~IDebugHandler()
	{
		Debugger::ForgetDebugTool(this);
	}

	bool IDebugHandler::IsEnabled() const
	{
#if _DEBUG
		return m_isEnabled;
#else
		return false;
#endif // _DEBUG
	}

	void IDebugHandler::OnEnabled()
	{
	}

	void IDebugHandler::OnDisabled()
	{
	}

	void IDebugHandler::HandleEnabledCheckbox()
	{
		bool enabled = m_isEnabled;
		if (ImGui::Checkbox("Enabled", &enabled))
		{
			m_isEnabled = enabled;

			if(m_isEnabled)
			{
				OnEnabled();
			}
			else
			{
				OnDisabled();
			}
		}
	}
}
