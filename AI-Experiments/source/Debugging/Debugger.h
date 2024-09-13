#pragma once

#include <list>
#include <map>
#include <string>
#include <vector>

#include <Structures/MultiValueStruct.h>

using std::list;
using std::map;
using std::pair;
using std::string;
using std::vector;

namespace aie
{
	class Renderer;
	class Window;
}

using aie::Renderer;
using aie::Window;

namespace Debugging
{
	class IDebugHandler;

	enum EVerbosity
	{
		Basic,
		Advanced
	};

	class Debugger
	{
		friend class AiApplication;

	private:
		typedef void(Debugger::* ListChange)(IDebugHandler*);

	public:
		static void ObserveDebugTool(IDebugHandler* handler);
		static void ForgetDebugTool(IDebugHandler* handler);
		static void SetVerbosity(EVerbosity verbosity);
		static bool InDebugMode();

		static bool HasDebugValue(const string& key);

		static void Set(const string& key, float fValue);
		static void Set(const string& key, int iValue);
		static void Set(const string& key, char cValue);
		static void Set(const string& key, bool bValue);
		static void Set(const string& key, vec2 v2Value);
		static void Set(const string& key, vec3 v3Value);
		static void Set(const string& key, vec4 v4Value);
		static void Set(const string& key, void* pValue);

		static bool Get(const string& key, float& fValue);
		static bool Get(const string& key, int& iValue);
		static bool Get(const string& key, char& cValue);
		static bool Get(const string& key, bool& bValue);
		static bool Get(const string& key, vec2& v2Value);
		static bool Get(const string& key, vec3& v3Value);
		static bool Get(const string& key, vec4& v4Value);
		static bool Get(const string& key, void*& pValue);

	private:
		static void Create(Window* window);
		static void Destroy();

	private:
		static Debugger* m_instance;

	private:
		list<pair<ListChange, IDebugHandler*>> m_changes;
		list<IDebugHandler*> m_handlers;

		map<string, MultiValueStruct*> m_debugValues;

		EVerbosity m_verbosity;
		bool m_debugModeEnabled;

		Window* m_window;

	private:
		Debugger();
		~Debugger();

	private:
		void Tick();
		void Render(Renderer* renderer);

		void AddHandler(IDebugHandler* handler);
		void RemoveHandler(IDebugHandler* handler);

	};

	extern string MakeId(const string& category, const vector<string>& identifiers);
}