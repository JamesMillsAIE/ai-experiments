#include "IDebugHandler.h"

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

	void IDebugHandler::RenderDebuggingTools(Renderer2D* renderer, const EVerbosity verbosity)
	{
		OnRenderDebuggingTools(renderer, verbosity);
	}

}
