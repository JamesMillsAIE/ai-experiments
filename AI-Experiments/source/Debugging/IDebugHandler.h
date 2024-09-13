#pragma once

#include <string>

#include "Debugger.h"

using std::string;

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

		virtual void RenderDebuggingTools(Renderer* renderer, EVerbosity verbosity) = 0;
		virtual void HandleImGui(EVerbosity verbosity) = 0;

		virtual void OnEnabled();
		virtual void OnDisabled();

	private:
		void HandleEnabledCheckbox();

	};

}