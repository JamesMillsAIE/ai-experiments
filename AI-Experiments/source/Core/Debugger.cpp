#include "Debugger.h"

#include <imgui.h>

#include "GameTime.h"
#include "IDebugHandler.h"
#include "Input.h"
#include "MultiValueStruct.h"

#include <sstream>

using std::stringstream;

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

	bool Debugger::HasDebugValue(const string& key)
	{
#if _DEBUG
		if (m_instance)
		{
			return m_instance->m_debugValues.contains(key);
		}
#endif

		return false;
	}

	void Debugger::Set(const string& key, const float fValue)
	{
#if _DEBUG
		if (!HasDebugValue(key))
		{
			m_instance->m_debugValues[key] = new MultiValueStruct;
		}

		m_instance->m_debugValues[key]->type = Float;
		m_instance->m_debugValues[key]->fValue = fValue;
#endif // _DEBUG

	}

	void Debugger::Set(const string& key, const int iValue)
	{
#if _DEBUG
		if (!HasDebugValue(key))
		{
			m_instance->m_debugValues[key] = new MultiValueStruct;
		}

		m_instance->m_debugValues[key]->type = Int;
		m_instance->m_debugValues[key]->iValue = iValue;
#endif // _DEBUG
	}

	void Debugger::Set(const string& key, const char cValue)
	{
#if _DEBUG
		if (!HasDebugValue(key))
		{
			m_instance->m_debugValues[key] = new MultiValueStruct;
		}

		m_instance->m_debugValues[key]->type = Char;
		m_instance->m_debugValues[key]->cValue = cValue;
#endif // _DEBUG
	}

	void Debugger::Set(const string& key, const bool bValue)
	{
#if _DEBUG
		if (!HasDebugValue(key))
		{
			m_instance->m_debugValues[key] = new MultiValueStruct;
		}

		m_instance->m_debugValues[key]->type = Bool;
		m_instance->m_debugValues[key]->bValue = bValue;
#endif // _DEBUG
	}

	void Debugger::Set(const string& key, const vec2 v2Value)
	{
#if _DEBUG
		if (!HasDebugValue(key))
		{
			m_instance->m_debugValues[key] = new MultiValueStruct;
		}

		m_instance->m_debugValues[key]->type = Vec2;
		m_instance->m_debugValues[key]->v2Value = v2Value;
#endif // _DEBUG
	}

	void Debugger::Set(const string& key, const vec3 v3Value)
	{
#if _DEBUG
		if (!HasDebugValue(key))
		{
			m_instance->m_debugValues[key] = new MultiValueStruct;
		}

		m_instance->m_debugValues[key]->type = Vec3;
		m_instance->m_debugValues[key]->v3Value = v3Value;
#endif // _DEBUG
	}

	void Debugger::Set(const string& key, const vec4 v4Value)
	{
#if _DEBUG
		if (!HasDebugValue(key))
		{
			m_instance->m_debugValues[key] = new MultiValueStruct;
		}

		m_instance->m_debugValues[key]->type = Vec4;
		m_instance->m_debugValues[key]->v4Value = v4Value;
#endif // _DEBUG
	}

	void Debugger::Set(const string& key, void* pValue)
	{
#if _DEBUG
		if (!HasDebugValue(key))
		{
			m_instance->m_debugValues[key] = new MultiValueStruct;
		}

		m_instance->m_debugValues[key]->type = Pointer;
		m_instance->m_debugValues[key]->pValue = pValue;
#endif // _DEBUG
	}

	bool Debugger::Get(const string& key, float& fValue)
	{
#if _DEBUG
		if (HasDebugValue(key))
		{
			MultiValueStruct* debugValue = m_instance->m_debugValues[key];
			if (debugValue->type == Float)
			{
				fValue = debugValue->fValue;
				return true;
			}
		}
#endif

		return false;
	}

	bool Debugger::Get(const string& key, int& iValue)
	{
#if _DEBUG
		if (HasDebugValue(key))
		{
			MultiValueStruct* debugValue = m_instance->m_debugValues[key];
			if (debugValue->type == Int)
			{
				iValue = debugValue->iValue;
				return true;
			}
		}
#endif

		return false;
	}

	bool Debugger::Get(const string& key, char& cValue)
	{
#if _DEBUG
		if (HasDebugValue(key))
		{
			MultiValueStruct* debugValue = m_instance->m_debugValues[key];
			if (debugValue->type == Char)
			{
				cValue = debugValue->cValue;
				return true;
			}
		}
#endif

		return false;
	}

	bool Debugger::Get(const string& key, bool& bValue)
	{
#if _DEBUG
		if (HasDebugValue(key))
		{
			MultiValueStruct* debugValue = m_instance->m_debugValues[key];
			if (debugValue->type == Bool)
			{
				bValue = debugValue->bValue;
				return true;
			}
		}
#endif

		return false;
	}

	bool Debugger::Get(const string& key, vec2& v2Value)
	{
#if _DEBUG
		if (HasDebugValue(key))
		{
			MultiValueStruct* debugValue = m_instance->m_debugValues[key];
			if (debugValue->type == Vec2)
			{
				v2Value = debugValue->v2Value;
				return true;
			}
		}
#endif

		return false;
	}

	bool Debugger::Get(const string& key, vec3& v3Value)
	{
#if _DEBUG
		if (HasDebugValue(key))
		{
			MultiValueStruct* debugValue = m_instance->m_debugValues[key];
			if (debugValue->type == Vec3)
			{
				v3Value = debugValue->v3Value;
				return true;
			}
		}
#endif

		return false;
	}

	bool Debugger::Get(const string& key, vec4& v4Value)
	{
#if _DEBUG
		if (HasDebugValue(key))
		{
			MultiValueStruct* debugValue = m_instance->m_debugValues[key];
			if (debugValue->type == Vec4)
			{
				v4Value = debugValue->v4Value;
				return true;
			}
		}
#endif

		return false;
	}

	bool Debugger::Get(const string& key, void*& pValue)
	{
#if _DEBUG
		if (HasDebugValue(key))
		{
			MultiValueStruct* debugValue = m_instance->m_debugValues[key];
			if (debugValue->type == Pointer)
			{
				pValue = debugValue->pValue;
				return true;
			}
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
			for (const auto& handler : m_handlers)
			{
				if(handler->IsEnabled())
				{
					handler->RenderDebuggingTools(renderer, m_verbosity);
				}
			}

			if (ImGui::Begin("Debugger"))
			{
				ImGui::Text("FPS: %d", GameTime::FPS());

				const char* verbosityValues[] =
				{
					 "Basic",
					 "Advanced"
				};

				int verbosity = m_verbosity;
				ImGui::Combo("Verbosity", &verbosity, verbosityValues, 2);
				m_verbosity = static_cast<EVerbosity>(verbosity);

				for(const auto& handler : m_handlers)
				{
					if(ImGui::CollapsingHeader(handler->DebugCategory().c_str()))
					{
						ImGui::Checkbox("Enabled", &handler->m_isEnabled);

						handler->HandleImGui(m_verbosity);
					}
				}

				ImGui::End();
			}
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

string MakeId(const string& category, const vector<string>& identifiers)
{
	stringstream stream;

	stream << category;

	for (const auto& identifier : identifiers)
	{
		stream << ".";
		stream << identifier;
	}

	return stream.str();
}