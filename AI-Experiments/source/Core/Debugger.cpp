#include "Debugger.h"

#include <imgui/imgui.h>

#include "GameTime.h"
#include "IDebugHandler.h"
#include "Input.h"

namespace Debugging
{
	Debugger* Debugger::m_instance = nullptr;

	void Debugger::ObserveDebugTool(IDebugHandler* handler)
	{
#if _DEBUG
		if (m_instance)
		{
			m_instance->m_changes.emplace_back(&Debugger::AddHandler, handler);
		}
#endif
	}

	void Debugger::ForgetDebugTool(IDebugHandler* handler)
	{
#if _DEBUG
		if (m_instance)
		{
			m_instance->m_changes.emplace_back(&Debugger::RemoveHandler, handler);
		}
#endif
	}

	void Debugger::SetVerbosity(const EVerbosity verbosity)
	{
		if (m_instance)
		{
			m_instance->m_verbosity = verbosity;
		}
	}

	bool Debugger::IsDebuggingEnabled()
	{
#if _DEBUG
		if (m_instance)
		{
			return m_instance->m_isEnabled;
		}
#endif

		return false;
	}

	void Debugger::Create()
	{
		if (!m_instance)
		{
			m_instance = new Debugger;
		}
	}

	void Debugger::Destroy()
	{
		if (m_instance)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

	Debugger::Debugger() = default;

	Debugger::~Debugger() = default;

	void Debugger::Tick()
	{
#if _DEBUG
		for (auto& [change, handler] : m_changes)
		{
			(this->*change)(handler);
		}

		m_changes.clear();

		if (Input* input = Input::Get())
		{
			if (input->WasKeyPressed(KeyF3))
			{
				m_isEnabled = !m_isEnabled;
			}
		}
#endif // _DEBUG

	}

	void Debugger::Render(Renderer2D* renderer)
	{
#if	_DEBUG
		if (m_isEnabled)
		{
			ImGui::Begin("Debugger");
			const char* verbosityValues[] =
			{
				"Basic",
				"Advanced"
			};

			int verbosity = m_verbosity;
			ImGui::Combo("Verbosity", &verbosity, verbosityValues, 2);
			m_verbosity = static_cast<EVerbosity>(verbosity);
			ImGui::End();

			for (const auto& handler : m_handlers)
			{
				handler->RenderDebuggingTools(renderer, m_verbosity);
			}

			GameTime::RenderFPS(renderer);
		}
#endif
	}

	void Debugger::AddHandler(IDebugHandler* handler)
	{
		if (std::ranges::find(m_handlers, handler) != m_handlers.end())
		{
			return;
		}

		m_handlers.emplace_back(handler);
	}

	void Debugger::RemoveHandler(IDebugHandler* handler)
	{
		if (std::ranges::find(m_handlers, handler) == m_handlers.end())
		{
			return;
		}

		m_handlers.remove(handler);
	}
}
