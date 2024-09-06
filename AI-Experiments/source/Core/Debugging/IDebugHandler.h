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
		bool IsEnabled() const;
		virtual string DebugCategory() = 0;

		virtual void RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity) = 0;
		virtual void HandleImGui(EVerbosity verbosity) = 0;

		virtual void OnEnabled();
		virtual void OnDisabled();

	private:
		bool m_isEnabled;

	private:
		void HandleEnabledCheckbox();

	};

}