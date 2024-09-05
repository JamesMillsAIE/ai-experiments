#pragma once

#include "Debugger.h"

class Renderer2D;

namespace Debugging
{
	class IDebugHandler
	{
	public:
		IDebugHandler();
		virtual ~IDebugHandler();

	public:
		bool IsEnabled() const;
		void RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity);

	protected:
		virtual void OnRenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity) = 0;

	private:
		bool m_isEnabled;

	};

}