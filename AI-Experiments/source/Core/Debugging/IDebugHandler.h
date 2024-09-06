#pragma once

#include <string>

#include "Debugger.h"

using std::string;

class Renderer2D;

namespace Debugging
{
	class IDebugHandler
	{
		friend Debugger;

	public:
		IDebugHandler();
		virtual ~IDebugHandler();

	protected:
		bool m_isDebuggingEnabled;

	protected:
		bool IsDebuggingEnabled() const;
		virtual string DebugCategory() = 0;

		virtual void RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity) = 0;
		virtual void HandleImGui(EVerbosity verbosity) = 0;

		virtual void OnEnabled();
		virtual void OnDisabled();

	private:
		void HandleEnabledCheckbox();

	};

}