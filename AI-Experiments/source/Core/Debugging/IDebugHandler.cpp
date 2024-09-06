#include "IDebugHandler.h"

#include "imgui.h"

namespace Debugging
{
	IDebugHandler::IDebugHandler()
		: m_isDebuggingEnabled{ false }
	{
		Debugger::ObserveDebugTool(this);
	}

	IDebugHandler::~IDebugHandler()
	{
		Debugger::ForgetDebugTool(this);
	}

	bool IDebugHandler::IsDebuggingEnabled() const
	{
#if _DEBUG
		return m_isDebuggingEnabled;
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
		bool enabled = m_isDebuggingEnabled;
		if (ImGui::Checkbox("Enabled", &enabled))
		{
			m_isDebuggingEnabled = enabled;

			if(m_isDebuggingEnabled)
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
